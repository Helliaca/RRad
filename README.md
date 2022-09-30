# RRad

An implementation of progressive radiosity that runs on the GPU.

It serves as a demonstration for how visibility-determination is the only major hurdle that prevents radiosity from easily running on the GPU.
The implementation is simple and utilizes discrete ray-intersections inside the fragment shader to determine visibility.

![Example image](https://i.imgur.com/YhNoIDM.jpg)

## Overview

The program generates a set of textures that contain input data. Namely: normal vector, position and area-fraction.
The emissive color is stored into two textures `ligTex0` and `ligTex1`. With each pass, the output lighting is written to one of these textures, with the other serving as the input.

The radiosity pass is performed by the [make_lightmap](https://github.com/Helliaca/RRad/blob/master/SimpleRAD/shaders/make_lightmap.fs) shader.
This shader includes a `occluded(A, B)` function that determines if two locations are mutually visible. Visibility is determined through explicit intersection functions (`ray_cone`, `ray_cube` and `ray_sphere`) which are executed for 'virtual' shapes that are hard-coded into the shader. You can view and modify these inside the `occluded` function.

## Dependencies

- [Assimp 3.1.1](https://github.com/assimp/assimp/releases/tag/v3.3.1/)
- [GLFW 3.2.1](https://www.glfw.org/download.html)
- [GLM 9.8.5](https://github.com/g-truc/glm/releases/tag/0.9.8.5)
- [An OpenGL 4.5 loader generated with glad 0.1.16a0](http://glad.dav1d.de/#profile=core&language=c&specification=gl&loader=on&api=gl%3D4.5)

## Manual

Once you start the program, you can use the separate commandline to control execution. Available commands:

- `pass` to run a radiosity pass - ie. simulate a bounce of light.
- `phong` to show direct lighting via a standard phong shader.
- `res` or `showres` to show the texture resolution as a black-and-white checkers pattern.
- `tex` to show the texture in its UV-coordinates.
- `obj` to show the texture mapped onto the underlying 3D model.
- `mode` to toggle between the two former modes.
- `tex [texturename]` to display a given input-texture. Avalilable parameters: `posTex`, `nrmTex`, `arfTex`, `ligTex1`.
