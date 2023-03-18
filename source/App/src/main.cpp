#include "Debug/Log.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Application.h"

// settings
constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;"
"layout(location = 1) in vec3 aColor;"
"layout(location = 2) in vec2 aTexCoord;"

"out vec3 ourColor;"
"out vec2 TexCoord;"

"void main()"
"{"
"	gl_Position = vec4(aPos, 1.0);"
"	ourColor = aColor;"
"	TexCoord = aTexCoord;"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;"

"in vec3 ourColor;"
"in vec2 TexCoord;"
"uniform sampler2D texture1;"
"uniform sampler2D texture2;"
"void main()"
"{"
"	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);"
"}\0";

int main()
{
	const My::Application app(SCR_WIDTH, SCR_HEIGHT, "Modern OpenGL Project - Loick");
	app.run();

	return 0;
}
