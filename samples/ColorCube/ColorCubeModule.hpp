#ifndef COLORCUBEMODULE_HPP
#define COLORCUBEMODULE_HPP

#include <XPG/Window.hpp>
#include <XPG/Matrix4x4.hpp>
#include <XPG/OpenGL/InterleavedBufferObject.hpp>
#include <XPG/OpenGL/Program.hpp>

class ColorCubeModule
{
    public:
        ColorCubeModule();
        virtual ~ColorCubeModule();

        void Open(XPG::Window& window);

    protected:
    private:
        XPG::Window* _window;
        XPG::InterleavedBufferObject _vertices;
        XPG::IndexBufferObject _indices;
        XPG::Program _program;
        GLint _matrixUniform;

        XPG::Matrix4x4<GLfloat> _projection;
        XPG::Matrix4x4<GLfloat> _modelView;

        static void OnKeyDown(XPG::Key::Code key, void* userData);
};

#endif
