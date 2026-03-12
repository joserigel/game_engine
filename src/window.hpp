#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include <GLFW/glfw3.h>

#define WINDOW_DEFAULT_WIDTH 800
#define WINDOW_DEFAULT_HEIGHT 600

class Window {
    private:
        GLFWwindow* _ID;
    public:
        ~Window();
        Window();

        GLFWwindow* ID();
        void run();
};

#endif
