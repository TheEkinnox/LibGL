#include "DemoApp.h"

#include <crtdbg.h>

// settings
constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	LibGL::Demo::DemoApp app(SCR_WIDTH, SCR_HEIGHT, "Modern OpenGL Project - Loick");
	app.run();

	return 0;
}
