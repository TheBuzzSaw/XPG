#include "ColorCubeModule.hpp"
#include <XPG/Application.hpp>

int main(int argc, char** argv)
{
    XPG::Application application;
    XPG::Window window;

    {
        ColorCubeModule module;
        module.Open(window);

        application.Run();
    }

    return 0;
}
