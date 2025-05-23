# OpenGL Solar System Simulation

A 3D solar system simulation created with OpenGL, GLUT, and GLEW.

## Overview

This project demonstrates the use of OpenGL to create a 3D simulation of planets revolving around a central star. The simulation includes rotation and revolution animations for each planet with customizable speeds.

## Features

- 3D rendering of planets in a solar system
- Individual rotation and revolution animations for each celestial body
- Pause/resume functionality with spacebar
- Camera controls for navigating the 3D space
- Lighting effects to simulate sunlight

## Controls

- **ESC**: Exit the application
- **Spacebar**: Pause/resume the animation
- **Mouse**: Camera control

## Dependencies

- **OpenGL**: Core graphics library
- **GLUT**: The OpenGL Utility Toolkit for window creation and input handling
- **GLEW**: The OpenGL Extension Wrangler Library (version 2.1.0 or 2.2.0) for accessing modern OpenGL features

## Building the Project

### Windows with MinGW

g++ -o SolarSystem main.cpp -lopengl32 -lfreeglut -lglew32

g++ -o SolarSystem main.cpp -lGL -lGLU -lglut -lGLEW

g++ -o SolarSystem main.cpp -framework OpenGL -framework GLUT -lGLEW

## Structure

- `main.cpp`: Contains the core application logic, rendering, and animation code
- `glew-2.1.0/` and `glew-2.2.0-win32/`: GLEW library files
- `freeglut.dll`: GLUT runtime library for Windows

## Implementation Details

The application implements:

- Vertex and fragment shaders for rendering
- Matrix transformations for planet positioning and animation
- Object loading and texture mapping
- Dynamic lighting calculations

## License

The project uses the following libraries:

- GLEW: Custom BSD-like license (see `glew-2.1.0/doc/credits.html`)
- GLUT: Open-source license

## Acknowledgements

This project uses:

- The OpenGL Extension Wrangler Library (GLEW)
- The OpenGL Utility Toolkit (GLUT/FreeGLUT)
