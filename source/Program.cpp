#include "../include/XPG/OpenGL/Program.hpp"
#include <cassert>

namespace XPG
{
    Program::Program()
    {
        _handle = glCreateProgram();
        _shaderCount = 0;
        _isLinked = false;
    }

    Program::~Program()
    {
        for (GLuint i = 0; i < _shaderCount; ++i)
            glDetachShader(_handle, _shaders[i]);

        glDeleteProgram(_handle);
    }

    void Program::Attach(const Shader& shader)
    {
        assert(!_isLinked);
        assert(_shaderCount < MaxShaders);

        _shaders[_shaderCount++] = shader._handle;
        glAttachShader(_handle, shader._handle);
    }

    bool Program::Link()
    {
        bool success = false;
        assert(!_isLinked);

        glLinkProgram(_handle);

        GLint result;
        glGetProgramiv(_handle, GL_LINK_STATUS, &result);

        if (result)
        {
            success = true;
            _isLinked = true;
            Use();
        }

        return success;
    }

    void Program::Use()
    {
        assert(_isLinked);
        glUseProgram(_handle);
    }

    void Program::BindAttribLocation(GLuint index, const GLchar* name)
    {
        assert(!_isLinked);
        glBindAttribLocation(_handle, index, name);
    }

    GLint Program::GetUniformLocation(const GLchar* name)
    {
        assert(_isLinked);
        return glGetUniformLocation(_handle, name);
    }
}
