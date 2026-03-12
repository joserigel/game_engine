#include "window.hpp"

Window::Window() {
    _ID = glfwCreateWindow(
            WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT, 
            "GameEngine", nullptr, nullptr);
    glfwMakeContextCurrent(_ID);
    glfwSetInputMode(_ID, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glViewport(0, 0, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT);
}

Window::~Window() {
}

void Window::run() {
    while (!glfwWindowShouldClose(_ID)) {
        glfwPollEvents();
    }
}

GLFWwindow* Window::ID() {
    return _ID;
}
