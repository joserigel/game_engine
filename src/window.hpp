#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "camera.hpp"
#include "shader.hpp"
#include "model.hpp"

#define WINDOW_DEFAULT_WIDTH 800
#define WINDOW_DEFAULT_HEIGHT 600
#define WINDOW_DEFAULT_FOV 90.0f

using namespace std;

class Window {
    private:
        GLFWwindow* _id;
        void _keyboardEvent(float delta);
        Camera _camera;
        unsigned int _screenTexture;
        unsigned int _frameBuffer;
        unsigned int _rbo;
        unsigned int _screenVAO;

        unique_ptr<Shader> _objectShader;
        unique_ptr<Shader> _screenShader;

        vector<Model> _models;

        static void _sizeCallback(
            GLFWwindow* window, int width, int height);
        static void _cursorPosCallback(
            GLFWwindow* window, double xpos, double ypos);

        void _drawScene();
        void _drawScreen();
    public:
        ~Window();
        Window();

        GLFWwindow* id();
        void run();
};

#endif
