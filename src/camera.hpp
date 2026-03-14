#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera {
    private:
        glm::mat4 _projection;
        glm::mat4 _view;
        float _fov;
        float _aspectRatio;

        glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f);
        float _pitch = 0;
        float _yaw = 0;

        float _lookSensitivity = 0.1f;
        float _moveSensitivity = 0.01f;

        double _lastX;
        double _lastY;
        double _firstMouse = true;
    public:
        Camera(float fov, int width, int height);
        void setAspectRatio(int width, int height);
        void mouseCallback(double xpos, double ypos);
        void keyboardCallback(GLFWwindow* window, float delta);
        glm::mat4 matrix();
};

#endif
