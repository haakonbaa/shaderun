# shaderun

A tool for running GLSL shaders. Written using GLFW, GLEW and openGL.

## Example

```bash
shaderun fragment_shader.glsl
```

This will open a window and display the shader as defined in **fragment_shader.glsl**.

## Installation

Required dependencies are **libglew-dev**, **libglfw3-dev** and **libgl1-mesa-dev**
```bash
sudo apt-get update
sudo apt-get install libglew-dev libglfw3-dev libgl1-mesa-dev
```

Compile with **g++**:
```bash
g++ -o shaderun shaderun.cpp -lglfw -lGLEW -lGL
```

If you are on Windows, follow [this](https://www.wikihow.com/Uninstall-Windows) tutorial.

## Usage

```txt
USAGE: shaderun [-w <width>] [-h <height>] [-v <vertex file>] <fragment-shader>
arguments:
    -w | --width     width of screen in pixels (default 800)
    -h | --height    height of screen in pixels (default 800)
    -v | --vertex    file containing vertex shader
    fragment-shader  file containing fragment shader

By default, the fragment shader has access to the following variables:
in vec3 texCoord;    // coordinates (x, y, 0) x, y are in [-1, 1]
out vec4 fragColor;  // pixel color
uniform float uTime; // time since animation started in seconds


The default vertex shader is

#version 330 core
layout(location = 0) in vec3 aPos;
out vec3 texCoord;
void main()
{
    gl_Position = vec4(aPos, 1.0);
    texCoord = vec3(aPos.xy, 0.0);
}
```
