#ifndef __RENDERING_CAMERA_HPP__
#define __RENDERING_CAMERA_HPP__


#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


class Camera {
    private:
        glm::mat4 projection_;
        glm::mat4 view_;
        float fov_;
        float aspectRatio_;

        glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f);
        float pitch_ = 0;
        float yaw_ = 0;

        float lookSensitivity_ = 0.1f;
        float moveSensitivity_ = 1.0f;

        double lastX_;
        double lastY_;
        double firstMouse_ = true;
    public:
        Camera(float fov = 90.0f, int width = 100, int height = 100);
        void setAspectRatio(int width, int height);
        void mouseCallback(GLFWwindow* id);
        void keyboardCallback(GLFWwindow* window, float delta);

        glm::vec3 position();
        glm::mat4 matrix(bool translation = true);
};

#endif
