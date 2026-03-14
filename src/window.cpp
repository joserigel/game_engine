#include "window.hpp"

#include "shader.hpp"
#include "model.hpp"

#include <glm/gtc/type_ptr.hpp>

void Window::sizeCallback(GLFWwindow* window, int width, int height) {
    Window* windowObject = static_cast<Window*>(glfwGetWindowUserPointer(window));
    glViewport(0, 0, width, height);
    windowObject->_camera.setAspectRatio(width, height);
}

void Window::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    Window* windowObject = static_cast<Window*>(glfwGetWindowUserPointer(window));
    windowObject->_camera.mouseCallback(xpos, ypos);
}

Window::Window() : 
    _camera(WINDOW_DEFAULT_FOV, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT)
{
    _id = glfwCreateWindow(
            WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT, 
            "GameEngine", nullptr, nullptr);
    glfwMakeContextCurrent(_id);
    glfwSetInputMode(_id, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glViewport(0, 0, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT);
    
    glfwSetWindowUserPointer(_id, (void*)this);
    glfwSetFramebufferSizeCallback(_id, Window::sizeCallback);
    glfwSetCursorPosCallback(_id, Window::cursorPosCallback);
    glEnable(GL_DEPTH_TEST);
}

Window::~Window() {
}

void Window::_keyboardEvent(float delta) {
    if (glfwGetKey(_id, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(_id, true);
    }
    _camera.keyboardCallback(_id, delta);
}

void Window::run() {
    Shader shader(
            "../shaders/basic.vert", 
            "../shaders/basic.frag");

    Model model("../models/backpack/backpack.obj");

    float lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(_id)) {
        float currentTime = glfwGetTime();
        float delta = currentTime - lastTime; 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        _keyboardEvent(delta);

        unsigned int projectionLoc = shader.uniformLocation("projection");
        glUniformMatrix4fv(
                projectionLoc, 1, GL_FALSE, glm::value_ptr(_camera.matrix()));

        
        model.draw(shader);

        glfwPollEvents();
        glfwSwapBuffers(_id);
    }
}

GLFWwindow* Window::id() {
    return _id;
}
