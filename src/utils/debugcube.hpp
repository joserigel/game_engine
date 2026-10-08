#ifndef __UTILS_DEBUGCUBE_HPP__
#define __UTILS_DEBUGCUBE_HPP__

#include "../rendering/shader.hpp"

#define DEBUGCUBE_SIZE .01f


class DebugCube{
    private:
        Shader shader_;
        unsigned int vao_;
        unsigned int vbo_;
    public:
        DebugCube();
        void draw(glm::mat4& projection, glm::vec3& position, glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f));
        void drawLine(glm::mat4& projection, glm::vec3 a, glm::vec3 b, glm::vec3 color = glm::vec3(0.0f, 0.0f, 1.0f));
};

#endif
