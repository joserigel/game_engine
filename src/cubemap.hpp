#ifndef __CUBEMAP_HPP__
#define __CUBEMAP_HPP__

#include <glm/matrix.hpp>

#include "shader.hpp"

class CubeMap {
    private:
        unsigned int _texture;
        unsigned int _vao;
        Shader _shader;
    public:
        unsigned int id();
        void draw(glm::mat4& matrix);

        CubeMap(
            const char* left,
            const char* right,
            const char* bottom,
            const char* top,
            const char* front,
            const char* back
            );
};

#endif
