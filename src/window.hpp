#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "camera.hpp"

#define WINDOW_DEFAULT_WIDTH 800
#define WINDOW_DEFAULT_HEIGHT 600
#define WINDOW_DEFAULT_FOV 90.0f

class Window {
    private:
        GLFWwindow* _id;
        void _keyboardEvent(float delta);
        Camera _camera;

        static void sizeCallback(
            GLFWwindow* window, int height, int width);
        static void cursorPosCallback(
            GLFWwindow* window, double xpos, double ypos);
    public:
        ~Window();
        Window();

        GLFWwindow* id();
        void run();
};

#endif
