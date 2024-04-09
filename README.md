# LibGL

> A lightweight C++ OpenGL game development library

[![Build](https://github.com/TheEkinnox/LibGL/actions/workflows/main.yml/badge.svg)](https://github.com/TheEkinnox/LibGL/actions/workflows/main.yml)

___

### Creation context

Created in 3 weeks from March 13th to 31st 2023 during my second semester in [Game Engine Programming](https://www.isart.ca/programmes/programmes-courts/programmeur-moteur-jeu-video/) at [Isart Digital](https://www.isart.ca/).

### My goal

My goal for this project was to create a C++ rendering library using OpenGL targeted towards video game development.

### Project's description

The library offers useful features for game development, as well as a simple demo project. To avoid unnecessary compile times and code bloat, the library has been split in five (5) distinct modules, each handling a specific set of capabilities :

- `Core`: Graph data structure, Logging, Assertions, Eventing, Resource Management, Service Locator, Thread Pool, Timer, Common utility functions
- `App`: Windowing, Input management, Application context
- `Entities`: Component-based scene management
- `Physics`: Simple rigid body physics without angular velocity, Ray casting, Collider overlap testing (similar to [Unity's](https://unity.com/) `Physics.Overlap` functions)
- `Rendering`: OpenGL Rendering Engine, Blinn-Phong shading, Shadow Mapping

Since the main focus of this project was the renderer, I used the following libraries to facilitate some parts of the development:

- [GLFW](https://github.com/glfw/glfw) (Windowing and inputs)
- [Glad](https://github.com/Dav1dde/glad) (OpenGL loader)
- [STB Image](https://github.com/nothings/stb) (Image loader)
- [LibMath](https://github.com/TheEkinnox/LibMath) (In-House math library)

### What I learned

Creating a modular rendering library in C++ using OpenGL allowed me to deepen my knowledge of C++, while also consolidating my planning and organization skills. Furthermore, this exercise was a great opportunity to learn more about the various challenges of library design and software architecture. Finally, I learned a lot about graphics programming and OpenGL, from GLSL shader development to useful rendering techniques such as shadow mapping.

### Project's progression :

The library is, although constantly being improved, usable, and has been used to create [a 3D puzzle game](https://noxcorporation.net/en/project/pfa).
