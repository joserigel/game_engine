#ifndef __CUBEMAP_HPP__
#define __CUBEMAP_HPP__

#include <glm/matrix.hpp>

#include "shader.hpp"

class CubeMap {
    private:
        unsigned int texture_;
        unsigned int vao_;
        Shader shader_;
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
