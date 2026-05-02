#ifndef __LIGHT_HPP__
#define __LIGHT_HPP__

#include <glm/glm.hpp>

#define DEFAULT_SHADOW_WIDTH 1024
#define DEFAULT_SHADOW_HEIGHT 1024
#define DEFAULT_CAMERA_DIR glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f))

class DirectionalLight {
    private:
        unsigned int shadowTexture_;
        unsigned int fbo_;
        glm::vec3 direction_;
        glm::mat4 matrix_;
    public:
        DirectionalLight(glm::vec3 direction = DEFAULT_CAMERA_DIR);

        unsigned int shadowTexture();
        unsigned int fbo();
        glm::mat4 matrix();
        glm::vec3 direction();
        void setDirection(glm::vec3& dir);
};


#endif
