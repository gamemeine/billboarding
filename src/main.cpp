
#include "Window.h"

int main()
{
    Program::setProgramsDirectory("../res/shaders");

    Window window;
    window.Init(1280, 720, "Billboarding");
    window.RunMainLoop();
}
