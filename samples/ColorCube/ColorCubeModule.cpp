#include "ColorCubeModule.hpp"

#include <iostream>
using namespace std;

static const GLuint PositionHandle = 0;
static const GLuint ColorHandle = 1;

ColorCubeModule::ColorCubeModule()
{
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    GLfloat cubeBuffer[] = {
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f };

    GLushort cubeIndices[] = {
        0, 3, 7, 0, 7, 4,
        4, 7, 6, 4, 6, 5,
        5, 6, 2, 5, 2 ,1,
        1, 2, 3, 1, 3, 0,
        3, 2, 6, 3, 6, 7,
        4, 5, 1, 4, 1, 0 };

    _vertices.Add(PositionHandle, 3);
    _vertices.Add(ColorHandle, 3);
    _vertices.Load(cubeBuffer, 8);

    _indices.Load(cubeIndices, sizeof(*cubeIndices), 3, 12);

    XPG::Shader vs(GL_VERTEX_SHADER);

    const char* vsSource =
        "#version 150\n"
        "uniform mat4 uMVPM;\n"
        "in vec3 iPosition;\n"
        "in vec3 iColor;\n"
        "out vec3 vColor;\n"
        "void main()\n"
        "{\n"
        "    vColor = iColor;\n"
        "    vec4 p = vec4(iPosition, 1.0);\n"
        "    gl_Position = uMVPM * p;\n"
        "}\n"
        "\n";

    if (!vs.Load(vsSource))
    {
        cout << "error compiling vertex shader" << endl;
    }

    XPG::Shader fs(GL_FRAGMENT_SHADER);

    const char* fsSource =
        "#version 150\n"
        "in vec3 vColor;\n"
        "out vec4 oColor;\n"
        "void main(void)\n"
        "{\n"
        "    oColor = vec4(vColor, 1.0);\n"
        "}\n"
        "\n";

    if (!fs.Load(fsSource))
    {
        cout << "error compiling fragment shader" << endl;
    }

    _program.Attach(vs);
    _program.Attach(fs);
    _program.BindAttribLocation(PositionHandle, "iPosition");
    _program.BindAttribLocation(ColorHandle, "iColor");

    if (_program.Link())
    {
        _matrixUniform = _program.GetUniformLocation("uMVPM");
    }
    else
    {
        cout << "error linking program" << endl;
    }

    _resetViewport = false;
    glViewport(0, 0, 1024, 768);
    _projection.Perspective(30.0f, 1024.0f / 768.0f, 1.0f, 1000.0f);
    _modelView.Translate(0.0f, 0.0f, -10.0f);
    _modelView.RotateX(45.0f);
    _modelView.RotateY(45.0f);
    _vertices.EnableArrays();
    glBindVertexArray(0);

    _interval = XPG::TimeSpan::FromSeconds(1);
    _interval /= 120;
    _nextUpdate = XPG::ReadTimer();

    _rotation = 0.0f;

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
}

ColorCubeModule::~ColorCubeModule()
{
}

void ColorCubeModule::Open(XPG::Window& window)
{
    _window = &window;
    window.SetTitle("XPG Color Cube Demonstration");
    window.ClearAllEventCallbacks();
    window.UserData(this);
    window.OnKeyDown(OnKeyDown);
    window.OnClose(OnClose);
    window.OnResize(OnResize);
    window.SetVsync(false);

    window.MakeCurrent(false);
    _thread.Start(BeginRenderThread, this);
}

void ColorCubeModule::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    XPG::Matrix4x4<GLfloat> matrix(_projection, _modelView);
    glUniformMatrix4fv(_matrixUniform, 1, GL_FALSE, matrix);
    glBindVertexArray(_vao);
    _indices.DrawElements(GL_TRIANGLES);
    glBindVertexArray(0);
    _window->SwapBuffers();
}

void ColorCubeModule::Loop()
{
    const XPG::TimeSpan YieldInterval = XPG::TimeSpan::FromMilliseconds(1);

    cout << "calling MakeCurrent" << endl;
    _window->MakeCurrent(true);
    cout << "done calling MakeCurrent" << endl;

    _isRunning = true;

    while (_isRunning)
    {
        XPG::TimeSpan now = XPG::ReadTimer();

        _mutex.Lock();
        if (_resetViewport)
        {
            glViewport(0, 0, _state.Width(), _state.Height());
            _resetViewport = false;
        }
        _mutex.Unlock();

        while (now >= _nextUpdate)
        {
            _nextUpdate += _interval;

            OnUpdate();
        }

        Draw();


        XPG::Sleep(YieldInterval);
    }
}

void ColorCubeModule::OnUpdate()
{
    _rotation += 0.5f;
    if (_rotation > 180.0f) _rotation -= 360.0f;

    _modelView.loadIdentity();
    _modelView.Translate(0.0f, 0.0f, -10.0f);
    _modelView.RotateX(_rotation);
    _modelView.RotateY(_rotation);

}

void ColorCubeModule::Close()
{
    _isRunning = false;
    _thread.Join();
    _window->MakeCurrent(true);
    _window->Close();
}

void ColorCubeModule::OnKeyDown(XPG::Key::Code key, void* userData)
{
    if (key == XPG::Key::Escape)
    {
        ColorCubeModule* ccm = static_cast<ColorCubeModule*>(userData);
        ccm->Close();
    }
}

bool ColorCubeModule::OnClose(void* userData)
{
    ColorCubeModule* ccm = static_cast<ColorCubeModule*>(userData);
    ccm->Close();
    return false;
}

void ColorCubeModule::OnResize(const XPG::WindowState& state)
{
    ColorCubeModule* ccm = static_cast<ColorCubeModule*>(state.UserData());
    ccm->_state = state;
    float ratio = float(state.Width()) / float(state.Height());

    ccm->_mutex.Lock();
    ccm->_projection.loadIdentity();
    ccm->_projection.Perspective(30.0f, ratio, 1.0f, 1000.0f);
    ccm->_resetViewport = true;
    ccm->_mutex.Unlock();
}

void ColorCubeModule::BeginRenderThread(void* userData)
{
    ColorCubeModule* ccm = static_cast<ColorCubeModule*>(userData);
    ccm->Loop();
}
