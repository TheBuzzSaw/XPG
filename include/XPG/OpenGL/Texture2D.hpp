#ifndef XpgTexture2DHpp
#define XpgTexture2DHpp

#include "Texture.hpp"

namespace XPG
{
    XpgClass Texture2D : public Texture
    {
        public:
            Texture2D();
            virtual ~Texture2D();

            void LoadShadowMap(GLsizei width, GLsizei height);

        private:
    };
}

#endif
