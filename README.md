# Custom Game Engine
Name WIP

### Build and Run
```
mkdir build
cd build
cmake --build ..
./GameEngine
```
### Usage of Assimp
The `assimp` library is fetched using their Github page with CMake,
I put it here so that it's not a hassle to individually download and install,
but initial build time is then slow. Sorry :/

### stb
Loading textures uses [stb](https://github.com/nothings/stb?tab=readme-ov-file)

### Requirements
- OpenGL `sudo apt install libgl1-mesa-dev`
- GLEW  `sudo apt install libglew-dev`
- glfw3 `sudo apt install libglfw3-dev`

use these installations for Ubuntu, otherwise provide your own :D

#### Texture Conventions

Currently, the supported texture types are
`DIFFUSE`, `SPECULAR`, and `NORMALS`. The convention is that by default
you can declare `uniform sampler2D {type}_texture` in a shader file
to use the different types of textures. You can see usage example in the
`shaders/basic.frag` file. 

| Texture Type | Default GLSL Variable Name | Index |
| ------------ | -------------------------- | ----- |
| DIFFUSE      | diffuse_texture            | 0     |
| SPECULAR     | specular_texture           | 1     |
| NORMALS      | normals_texture            | 2     |





