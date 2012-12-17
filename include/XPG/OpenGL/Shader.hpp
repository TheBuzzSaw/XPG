#ifndef XpgShaderHpp
#define XpgShaderHpp

#include "Base.hpp"

namespace XPG
{
    XpgClass Shader
    {
        public:
            friend class Program;

            Shader(GLenum shaderType);
            virtual ~Shader();

            bool Load(const char* source);

        private:
            Shader(const Shader& other);
            Shader& operator=(const Shader& other);

            GLuint _handle;
    };
}

#endif
