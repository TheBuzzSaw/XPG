#include "../../include/XPG/Window.hpp"
#include "ApplicationMeta.hpp"

namespace XPG
{
    Window::Window()
    {
        memset(_native, 0, sizeof(_native));
        Open();
    }

    Window::~Window()
    {
        Close();
    }

    void Window::Open()
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);

        if (!meta->window)
        {
            meta->window = SDL_CreateWindow("XPG",
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                1024, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                                SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
            SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

            meta->context = SDL_GL_CreateContext(meta->window);

            meta->object = this;

            ++theApplicationMeta.windowCount;
            theApplicationMeta.windowMeta = meta;
        }
    }

    void Window::Close()
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);

        if (meta->window)
        {
            SDL_GL_DeleteContext(meta->context);
            SDL_DestroyWindow(meta->window);
            --theApplicationMeta.windowCount;
            memset(_native, 0, sizeof(_native));
        }
    }

    void Window::SetTitle(const char* title)
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);
        SDL_SetWindowTitle(meta->window, title);
    }

    void Window::MakeCurrent(bool enable)
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);

        if (enable)
        {
            SDL_GL_MakeCurrent(meta->window, meta->context);
        }
        else
        {
            SDL_GL_MakeCurrent(meta->window, 0);
        }
    }

    void Window::SwapBuffers()
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);
        SDL_GL_SwapWindow(meta->window);
    }

    void Window::SetVsync(bool enable)
    {
        SDL_GL_SetSwapInterval(enable ? 1 : 0);
    }

    const void* Window::UserData() const
    {
        const WindowMeta* meta = reinterpret_cast<const WindowMeta*>(_native);
        return meta->events.userData;
    }

    void Window::UserData(void* userData)
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);
        meta->events.userData = userData;
    }

    void Window::ClearAllEventCallbacks()
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);
        memset(&meta->events, 0, sizeof(meta->events));
    }

    void Window::OnLeftMouseButtonDown(MouseEventCallback callback)
    {
        EventBatch& batch = reinterpret_cast<WindowMeta*>(_native)->events;
        batch.onLeftMouseButtonDown = callback;
    }

    void Window::OnLeftMouseButtonUp(MouseEventCallback callback)
    {
        EventBatch& batch = reinterpret_cast<WindowMeta*>(_native)->events;
        batch.onLeftMouseButtonUp = callback;
    }

    void Window::OnMiddleMouseButtonDown(MouseEventCallback callback)
    {
        EventBatch& batch = reinterpret_cast<WindowMeta*>(_native)->events;
        batch.onMiddleMouseButtonDown = callback;
    }

    void Window::OnMiddleMouseButtonUp(MouseEventCallback callback)
    {
        EventBatch& batch = reinterpret_cast<WindowMeta*>(_native)->events;
        batch.onMiddleMouseButtonUp = callback;
    }

    void Window::OnRightMouseButtonDown(MouseEventCallback callback)
    {
        EventBatch& batch = reinterpret_cast<WindowMeta*>(_native)->events;
        batch.onRightMouseButtonDown = callback;
    }

    void Window::OnRightMouseButtonUp(MouseEventCallback callback)
    {
        EventBatch& batch = reinterpret_cast<WindowMeta*>(_native)->events;
        batch.onRightMouseButtonUp = callback;
    }

    void Window::OnMouseMove(MouseEventCallback callback)
    {
        EventBatch& batch = reinterpret_cast<WindowMeta*>(_native)->events;
        batch.onMouseMove = callback;
    }

    void Window::OnMouseWheelDown(MouseEventCallback callback)
    {
        EventBatch& batch = reinterpret_cast<WindowMeta*>(_native)->events;
        batch.onMouseWheelDown = callback;
    }

    void Window::OnMouseWheelUp(MouseEventCallback callback)
    {
        EventBatch& batch = reinterpret_cast<WindowMeta*>(_native)->events;
        batch.onMouseWheelUp = callback;
    }

    void Window::OnMouseExtraButtonDown(MouseExtraButtonEventCallback callback)
    {
        EventBatch& batch = reinterpret_cast<WindowMeta*>(_native)->events;
        batch.onMouseExtraButtonDown = callback;
    }

    void Window::OnMouseExtraButtonUp(MouseExtraButtonEventCallback callback)
    {
        EventBatch& batch = reinterpret_cast<WindowMeta*>(_native)->events;
        batch.onMouseExtraButtonUp = callback;
    }

    void Window::OnKeyDown(KeyboardEventCallback callback)
    {
        EventBatch& batch = reinterpret_cast<WindowMeta*>(_native)->events;
        batch.onKeyDown = callback;
    }

    void Window::OnKeyUp(KeyboardEventCallback callback)
    {
        EventBatch& batch = reinterpret_cast<WindowMeta*>(_native)->events;
        batch.onKeyUp = callback;
    }

    void Window::OnFocus(BasicCallback callback)
    {
        EventBatch& batch = reinterpret_cast<WindowMeta*>(_native)->events;
        batch.onWindowFocus = callback;
    }

    void Window::OnBlur(BasicCallback callback)
    {
        EventBatch& batch = reinterpret_cast<WindowMeta*>(_native)->events;
        batch.onWindowBlur = callback;
    }

    void Window::OnMouseIn(BasicCallback callback)
    {
        EventBatch& batch = reinterpret_cast<WindowMeta*>(_native)->events;
        batch.onWindowMouseIn = callback;
    }

    void Window::OnMouseOut(BasicCallback callback)
    {
        EventBatch& batch = reinterpret_cast<WindowMeta*>(_native)->events;
        batch.onWindowMouseOut = callback;
    }

    void Window::OnResize(WindowEventCallback callback)
    {
        EventBatch& batch = reinterpret_cast<WindowMeta*>(_native)->events;
        batch.onWindowResize = callback;
    }

    void Window::OnClose(SuccessCallback callback)
    {
        EventBatch& batch = reinterpret_cast<WindowMeta*>(_native)->events;
        batch.onWindowClose = callback;
    }

    void Window::OnExpose(BasicCallback callback)
    {
        EventBatch& batch = reinterpret_cast<WindowMeta*>(_native)->events;
        batch.onWindowExpose = callback;
    }
}
