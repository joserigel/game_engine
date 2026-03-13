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


