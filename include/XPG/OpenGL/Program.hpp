#ifndef XpgProgramHpp
#define XpgProgramHpp

#include "Shader.hpp"

namespace XPG
{
    XpgClass Program
    {
        public:
            Program();
            ~Program();

            void Attach(const Shader& shader);
            bool Link();
            void Use();
            void BindAttribLocation(GLuint index, const GLchar* name);
            GLint GetUniformLocation(const GLchar* name);

        private:
            Program(const Program& other);
            Program& operator=(const Program& other);

            static const GLuint MaxShaders = 16;

            GLuint _handle;
            GLuint _shaders[MaxShaders];
            GLuint _shaderCount;
            bool _isLinked;
    };
}

#endif
