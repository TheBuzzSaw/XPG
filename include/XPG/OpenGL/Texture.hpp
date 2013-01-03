#ifndef XpgTextureHpp
#define XpgTextureHpp

#include "Base.hpp"

namespace XPG
{
    XpgClass Texture
    {
        public:
            Texture(GLenum target);
            virtual ~Texture();

            void Bind();

        protected:
            void ProcessParams(const GLenum* params);

            inline GLenum Target() const { return _target; }

        private:
            Texture(const Texture& other);
            Texture& operator=(const Texture& other);

            GLenum _target;
            GLenum _handle;
    };
}

#endif

