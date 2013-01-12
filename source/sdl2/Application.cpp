#include "../../include/XPG/Application.hpp"
#include "../../include/XPG/WindowState.hpp"
#include "ApplicationMeta.hpp"
#include <SDL.h>

namespace XPG
{
    ApplicationMeta theApplicationMeta = {0};

    Application::Application()
    {
        if (theApplicationMeta.referenceCount < 1)
        {
            theApplicationMeta.referenceCount = 1;
            SDL_Init(SDL_INIT_VIDEO);
        }
        else
        {
            ++theApplicationMeta.referenceCount;
        }
    }

    Application::~Application()
    {
        if (theApplicationMeta.referenceCount < 2)
        {
            SDL_Quit();
            memset(&theApplicationMeta, 0, sizeof(theApplicationMeta));
        }
    }

    void Application::Run()
    {
        // I don't know how SDL2 distinguishes which events go to which window.
        // So, for now, it all goes to one window. I'll fix it later.

        SDL_Event event;

        while (theApplicationMeta.windowCount > 0
            && SDL_WaitEvent(&event) == 1)
        {
            WindowMeta* meta = theApplicationMeta.windowMeta;
            EventBatch& batch = meta->events;

            switch (event.type)
            {
                case SDL_QUIT:
                {
                    if (!batch.onWindowClose
                        || batch.onWindowClose(batch.userData))
                    {
                        meta->object->Close();
                    }

                    break;
                }

                case SDL_WINDOWEVENT:
                {
                    switch (event.window.event)
                    {
                        case SDL_WINDOWEVENT_EXPOSED:
                        {
                            if (batch.onWindowExpose)
                                batch.onWindowExpose(batch.userData);

                            break;
                        }

                        case SDL_WINDOWEVENT_RESIZED:
                        {
                            if (batch.onWindowResize)
                            {
                                WindowState state;
                                state.UserData(batch.userData);
                                state.Width(event.window.data1);
                                state.Height(event.window.data2);
                                batch.onWindowResize(state);
                            }

                            break;
                        }

                        case SDL_WINDOWEVENT_ENTER:
                        {
                            if (batch.onWindowMouseIn)
                                batch.onWindowMouseIn(batch.userData);

                            break;
                        }

                        case SDL_WINDOWEVENT_LEAVE:
                        {
                            if (batch.onWindowMouseOut)
                                batch.onWindowMouseOut(batch.userData);

                            break;
                        }

                        case SDL_WINDOWEVENT_FOCUS_GAINED:
                        {
                            if (batch.onWindowFocus)
                                batch.onWindowFocus(batch.userData);

                            break;
                        }

                        case SDL_WINDOWEVENT_FOCUS_LOST:
                        {
                            if (batch.onWindowBlur)
                                batch.onWindowBlur(batch.userData);

                            break;
                        }

                        case SDL_WINDOWEVENT_CLOSE:
                        {
                            if (!batch.onWindowClose
                                || batch.onWindowClose(batch.userData))
                            {
                                meta->object->Close();
                            }

                            break;
                        }

                        default: break;
                    }

                    break;
                }

                default: break;
            }
        }
    }
}
