#ifndef XpgWindowMetaHpp
#define XpgWindowMetaHpp

#include "../EventBatch.hpp"
#include "../../include/XPG/OpenGL/Base.hpp"
#include <SDL.h>

namespace XPG
{
    class Window;

    struct WindowMeta
    {
        EventBatch events;
        SDL_Window* window;
        SDL_GLContext context;
        Window* object;
    };
}

#endif
