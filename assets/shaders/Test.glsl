#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 ourColor;

void main()
{
	gl_Position = vec4(position, 1.0);
	ourColor = color;
}


#shader fragment
#version 330 core

in vec3 ourColor;

out vec4 gl_FragColor;

void main()
{
	gl_FragColor = vec4(ourColor, 1.0f);
}