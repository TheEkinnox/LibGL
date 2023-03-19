#include "Application.h"

// settings
constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

int main()
{
	const My::Application app(SCR_WIDTH, SCR_HEIGHT, "Modern OpenGL Project - Loick");
	app.run();

	return 0;
}
