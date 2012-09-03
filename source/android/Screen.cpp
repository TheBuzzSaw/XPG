#include "../../include/XPG/Screen.hpp"

#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

namespace XPG
{
  // struct to be able to reference our _native
  struct ScreenMeta
  {
        Screen* screen;
        android_app* app;
        ASensorManager* sensorManager;
        const ASensor* accelerometerSensor;
        ASensorEventQueue* sensorEventQueue;

        int animating;
        EGLDisplay display;
        EGLSurface surface;
        EGLContext context;
        int32_t width;
        int32_t height;
  };
  
  void GetCommand(struct android_app* app, int32_t command)
	{
	    ScreenMeta* meta = (ScreenMeta*)_native;
	    Screen* screen = (Screen*)meta->app->userData;
	    screen->OnCommand(meta->app, command);
	}

  Screen::Screen()
	{
	    ScreenMeta* meta = (ScreenMeta*)_native;
	    
	    meta->screen = this;
		meta->animating = 0;
		meta->display = EGL_NO_DISPLAY;
		meta->context = EGL_NO_CONTEXT;
		meta->surface = EGL_NO_SURFACE;

		meta->app->userData = this;
		meta->app->onAppCmd = GetCommand;
		meta->app->onInputEvent = GetInput;

		meta->sensorManager = ASensorManager_getInstance();
		meta->accelerometerSensor = ASensorManager_getDefaultSensor
			(meta->sensorManager, ASENSOR_TYPE_ACCELEROMETER);

		meta->sensorEventQueue = ASensorManager_createEventQueue(meta->sensorManager,
			meta->app->looper, LOOPER_ID_USER, NULL, NULL);

	}

	Screen::~Screen()
	{
		Close();
	}

	void Screen::Open()
	{
	    ScreenMeta* meta = (ScreenMeta*)_native;
		const EGLint attribs[] = {
				EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
				EGL_BLUE_SIZE, 8,
				EGL_GREEN_SIZE, 8,
				EGL_RED_SIZE, 8,
				EGL_NONE
		};

		meta->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

		eglInitialize(meta->display, 0, 0);

		/* Here, the application chooses the configuration it desires. In this
		 * sample, we have a very simplified selection process, where we pick
		 * the first EGLConfig that matches our criteria */
		eglChooseConfig(meta->display, attribs, &config, 1, &numConfigs);

		/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
		 * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
		 * As soon as we picked a EGLConfig, we can safely reconfigure the
		 * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
		eglGetConfigAttrib(meta->display, meta->config, EGL_NATIVE_VISUAL_ID, &format);

		ANativeWindow_setBuffersGeometry(meta->app->window, 0, 0, format);

		meta->surface = eglCreateWindowSurface(meta->display, meta->config, app->window, NULL);

		const EGLint contextAttributes[] =
			{
				EGL_CONTEXT_CLIENT_VERSION, 2,
				EGL_NONE
			};

		meta->context = eglCreateContext(meta->display, meta->config, EGL_NO_CONTEXT, contextAttributes);

		if (eglMakeCurrent(meta->display, meta->surface, meta->surface, meta->context) == EGL_FALSE) {
			LOGW("Unable to eglMakeCurrent");
			return;
		}

		eglQuerySurface(meta->display, meta->urface, EGL_WIDTH, &w);
		eglQuerySurface(meta->display, meta->surface, EGL_HEIGHT, &h);

		// Initialize GL state.
		glDisable(GL_DEPTH_TEST);

		LOGI("OpenGL Version: %s", glGetString(GL_VERSION));

		Event::Details details;
		details.source = this;
		ResizeDetails* rd = (ResizeDetails*)details.data;
		rd->width = meta->width;
		rd->height = meta->height;
		_onLoad.Fire(details);
	}

	void Screen::Close()
	{
	    ScreenMeta* meta = (ScreenMeta*)_native;
		meta->animating = 0;

		Event::Details details;
		details.source = this;
		_onUnload.Fire(details);

		if (meta->display != EGL_NO_DISPLAY)
		{
			eglMakeCurrent(meta->display, EGL_NO_SURFACE, EGL_NO_SURFACE,
				EGL_NO_CONTEXT);

			if (meta->context != EGL_NO_CONTEXT)
			{
				eglDestroyContext(meta->display, meta->context);
			}

			if (meta->surface != EGL_NO_SURFACE)
			{
				eglDestroySurface(meta->display, meta->surface);
			}

			eglTerminate(meta->display);
		}

		meta->animating = 0;
		meta->display = EGL_NO_DISPLAY;
		meta->context = EGL_NO_CONTEXT;
		meta->surface = EGL_NO_SURFACE;
	}

	void Screen::Run()
	{
	     ScreenMeta* meta = (ScreenMeta*)_native;
		while (1)
		{
			int ident;
			int events;

			struct android_poll_source* source;

			while ((ident=ALooper_pollAll(animating ? 0 : -1, NULL, &events,
				(void**)&source)) >= 0)
			{
				if (source != NULL)
				{
					source->process(app, source);
				}

				// If a sensor has data, process it now.
				if (ident == LOOPER_ID_USER)
				{
					if (meta->accelerometerSensor != NULL)
					{
						ASensorEvent event;
						while (ASensorEventQueue_getEvents(meta->sensorEventQueue,
							&event, 1) > 0)
						{
							//LOGI("accelerometer: x=%f y=%f z=%f",
							//		event.acceleration.x,
							//		event.acceleration.y,
							//		event.acceleration.z);
						}
					}
				}

				// Check if we are exiting.
				if (meta->app->destroyRequested != 0)
				{
					Close();
					return;
				}
			}

			if (meta->animating)
			{
				// Done with events; draw next animation frame.


				// Drawing is throttled to the screen update rate, so there
				// is no need to do timing here.
				Event::Details details;
                details.source = this;
				_onLoop.Fire(details);
			}
		}
	}

	void Screen::SwapBuffers()
	{
	     ScreenMeta* meta = (ScreenMeta*)_native;
		eglSwapBuffers(meta->display, meta->surface);
	}

	void Screen::Draw()
	{
	    ScreenMeta* meta = (ScreenMeta*)_native;
		if (meta->display != NULL)
		{
			Event::Details details;
            details.source = this;
			_onLoop.Fire(details);
		}
	}

	void Screen::OnCommand(struct android_app* app, int32_t command)
	{
	    ScreenMeta* meta = (ScreenMeta*)_native;
		LOGI("command %d", command);

	    switch (command)
	    {
			case APP_CMD_SAVE_STATE:
			{

			}

	        case APP_CMD_INIT_WINDOW:
	            // The window is being shown, get it ready.
	            if (app->window != NULL)
	            {
	                Open();
	                Draw();
	            }
	            break;

	        case APP_CMD_TERM_WINDOW:
	            // The window is being hidden or closed, clean it up.
	            Close();
	            break;

	        case APP_CMD_GAINED_FOCUS:
	            // When our app gains focus, we start monitoring the accelerometer.
	            if (accelerometerSensor != NULL) {
	                ASensorEventQueue_enableSensor(meta->sensorEventQueue,
						meta->accelerometerSensor);
	                // We'd like to get 60 events per second (in us).
	                ASensorEventQueue_setEventRate(meta->sensorEventQueue,
	                        meta->accelerometerSensor, (1000L/60)*1000);
	            }

	            meta->animating = 1;
	            break;

	        case APP_CMD_LOST_FOCUS:
	            // When our app loses focus, we stop monitoring the accelerometer.
	            // This is to avoid consuming battery while not being used.
	            if (meta->accelerometerSensor != NULL)
	            {
	                ASensorEventQueue_disableSensor(meta->sensorEventQueue,
						meta->accelerometerSensor);
	            }
	            // Also stop animating.
	            meta->animating = 0;
	            break;
	    }
	}

	int32_t Screen::OnInput(struct android_app* app, AInputEvent* event)
	{
	    ScreenMeta* meta = (ScreenMeta*)_native;
		if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
		{
			animating = 1;

			Event::Details details;
            details.source = this;
            ResizeDetails* rd = (ResizeDetails*)details.data;
            rd->width = meta->width;
            rd->height = meta->height;
			_onLoop.Fire(details);

			return 1;
		}

		return 0;
	}

	int32_t Screen::GetInput(struct android_app* app, AInputEvent* event)
	{
	    ScreenMeta* meta = (ScreenMeta*)_native;
		Screen* screen = (Screen*)meta->app->userData;
		return screen->OnInput(meta->app, event);
	}
}
