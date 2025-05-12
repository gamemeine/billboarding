
#include "Window.h"

int main()
{
    Program::setProgramsDirectory("C:/git/opemgl/res/shaders");

    Window window;
    window.Init(1280, 720, "Billboarding");
    window.RunMainLoop();
}
