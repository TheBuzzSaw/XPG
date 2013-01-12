#ifndef COLORCUBEMODULE_HPP
#define COLORCUBEMODULE_HPP

#include <XPG/Window.hpp>
#include <XPG/Clock.hpp>
#include <XPG/Thread.hpp>
#include <XPG/Matrix4x4.hpp>
#include <XPG/Mutex.hpp>
#include <XPG/DataTypes.hpp>
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
        void Draw();
        void Loop();
        void OnUpdate();
        void Close();

        XPG::Window* _window;
        XPG::InterleavedBufferObject _vertices;
        XPG::IndexBufferObject _indices;
        XPG::Program _program;
        GLuint _vao;
        GLint _matrixUniform;

        XPG::Matrix4x4<GLfloat> _projection;
        XPG::Matrix4x4<GLfloat> _modelView;
        float _rotation;
        XPG::TimeSpan _interval;
        XPG::TimeSpan _nextUpdate;
        XPG::Thread _thread;
        XPG::Mutex _mutex;
        XPG::WindowState _state;
        volatile bool _isRunning;
        volatile bool _resetViewport;

        static void OnKeyDown(XPG::Key::Code key, void* userData);
        static bool OnClose(void* userData);
        static void OnResize(const XPG::WindowState& state);
        static void BeginRenderThread(void* userData);
};

#endif
