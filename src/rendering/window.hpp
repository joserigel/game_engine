#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "camera.hpp"
#include "cubemap.hpp"
#include "shader.hpp"
#include "model.hpp"
#include "light.hpp"

#define WINDOW_DEFAULT_WIDTH 800
#define WINDOW_DEFAULT_HEIGHT 600
#define WINDOW_DEFAULT_FOV 90.0f

using namespace std;

class Window {
    private:
        GLFWwindow* id_;
        void keyboardEvent_(float delta);
        int width_ = WINDOW_DEFAULT_WIDTH;
        int height_ = WINDOW_DEFAULT_HEIGHT;

        Camera camera_;

        unsigned int screenTexture_;
        unsigned int frameBuffer_;
        unsigned int rbo_;
        unsigned int screenVAO_;

        unique_ptr<Shader> objectShader_;
        unique_ptr<Shader> screenShader_;
        unique_ptr<Shader> shadowShader_;
        unique_ptr<CubeMap> skybox_;

        unique_ptr<DirectionalLight> light_;
        vector<Model> models_;

        static void sizeCallback_(
            GLFWwindow* window, int width, int height);
        static void cursorPosCallback_(
            GLFWwindow* window, double xpos, double ypos);

        void drawScene_();
        void drawShadow_();
        void drawScreen_();
    public:
        ~Window();
        Window();

        GLFWwindow* id();
        void run();
};

#endif
