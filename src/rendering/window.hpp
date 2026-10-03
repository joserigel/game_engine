#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "../utils/text.hpp"
#include "shader.hpp"

#define WINDOW_DEFAULT_WIDTH 800
#define WINDOW_DEFAULT_HEIGHT 600

using namespace std;

class Window {
    private:
        GLFWwindow* id_;
        void keyboardEvent_(float delta);
        int width_ = WINDOW_DEFAULT_WIDTH;
        int height_ = WINDOW_DEFAULT_HEIGHT;

        unsigned int screenTexture_;
        unsigned int frameBuffer_;
        unsigned int rbo_;
        unsigned int screenVAO_;

        shared_ptr<Shader> screenShader_;

        static void sizeCallback_(
            GLFWwindow* window, int width, int height);

        void drawScreen_();
    public:
        ~Window();
        Window();

        GLFWwindow* id();
        void run();
};

#endif
