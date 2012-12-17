#include "../include/XPG/OpenGL/Shader.hpp"
#include <cstdio>

namespace XPG
{
    Shader::Shader(GLenum shaderType)
    {
        _handle = glCreateShader(shaderType);
    }

    Shader::~Shader()
    {
        // http://www.opengl.org/sdk/docs/man/xhtml/glDeleteShader.xml

        // If a shader object to be deleted is attached to a program object, it
        // will be flagged for deletion, but it will not be deleted until it is
        // no longer attached to any program object, for any rendering context
        // (i.e., it must be detached from wherever it was attached before it
        // will be deleted). A value of 0 for shader will be silently ignored.

        glDeleteShader(_handle);
    }

    bool Shader::Load(const char* source)
    {
        bool success = false;

        if (source && *source)
        {
            const GLchar** buffer = &source;

            glShaderSource(_handle, 1, buffer, 0);
            glCompileShader(_handle);

            GLint result;
            glGetShaderiv(_handle, GL_COMPILE_STATUS, &result);

            if (result)
            {
                success = true;
            }
            else
            {
                GLchar log[2048];
                GLsizei length;
                glGetShaderInfoLog(_handle, 2048, &length, log);

                printf("%s\n", log);
            }
        }

        return success;
    }
}
