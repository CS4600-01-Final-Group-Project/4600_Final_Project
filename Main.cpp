#include "Main.h"

int main()
{

    SFMLApp app;
    app.run();
    return 0;
}

void SFMLApp::run()
{

    // Main game loop
    while (window.isOpen())
    {

        processEvents();
        update();
        render();
    }
}
