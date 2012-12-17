#include "ColorCubeModule.hpp"
#include <iostream>
using namespace std;

static const GLuint PositionHandle = 0;
static const GLuint ColorHandle = 1;

ColorCubeModule::ColorCubeModule()
{
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

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

    glViewport(0, 0, 640, 480);
    _projection.Perspective(30.0f, 640.0f / 480.0f, 1.0f, 1000.0f);
    _modelView.Translate(0.0f, 0.0f, -10.0f);
    _modelView.RotateX(45.0f);
    _modelView.RotateY(45.0f);

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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    XPG::Matrix4x4<GLfloat> matrix(_projection, _modelView);
    glUniformMatrix4fv(_matrixUniform, 1, GL_FALSE, matrix);
    _vertices.Draw(_indices);
    window.SwapBuffers();
}

void ColorCubeModule::OnKeyDown(XPG::Key::Code key, void* userData)
{
    ColorCubeModule* ccm = static_cast<ColorCubeModule*>(userData);

    if (key == XPG::Key::Escape)
        ccm->_window->Close();
}