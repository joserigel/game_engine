#include "window.hpp"

#include "model.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>

void Window::_sizeCallback(GLFWwindow* window, int width, int height) {
    Window* windowObject = static_cast<Window*>(glfwGetWindowUserPointer(window));
    glViewport(0, 0, width, height);
    windowObject->_camera.setAspectRatio(width, height);


    glBindTexture(GL_TEXTURE_2D, windowObject->_screenTexture);
    glTexImage2D(GL_TEXTURE_2D, 0,
            GL_RGB, width, height,
            0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glBindRenderbuffer(GL_RENDERBUFFER, windowObject->_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
            width, height);
}

void Window::_cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
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

    GLenum error_code = glewInit();
    if (error_code) {
        throw std::runtime_error("Cannot initialize glew");
    }


    _objectShader = make_unique<Shader>("../shaders/basic.vert", "../shaders/basic.frag");
    _screenShader = make_unique<Shader>("../shaders/screen.vert", "../shaders/screen.frag");
    _skybox = make_unique<CubeMap>(
        "../models/skybox/left.jpg",
        "../models/skybox/right.jpg",
        "../models/skybox/bottom.jpg",
        "../models/skybox/top.jpg",
        "../models/skybox/front.jpg",
        "../models/skybox/back.jpg"
        );
    _objectShader->use();
    unsigned int skyboxLoc = _objectShader->uniformLocation("skybox");
    glUniform1i(skyboxLoc, 0);
    _models.emplace_back("../models/backpack/backpack.obj");

    // Set cursor callback
    glViewport(0, 0, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT);
    glfwSetWindowUserPointer(_id, (void*)this);
    glfwSetFramebufferSizeCallback(_id, Window::_sizeCallback);
    glfwSetCursorPosCallback(_id, Window::_cursorPosCallback);
    glEnable(GL_DEPTH_TEST);

    // Create Frame buffer
    glGenFramebuffers(1, &_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);

    // Create screen texture
    glGenTextures(1, &_screenTexture);
    glBindTexture(GL_TEXTURE_2D, _screenTexture);
    glTexImage2D(GL_TEXTURE_2D, 0,
            GL_RGB, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT,
            0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D, _screenTexture, 0);

    // Create render buffer for depth and stencil
    glGenRenderbuffers(1, &_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
            WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_WIDTH);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, 
            GL_DEPTH_STENCIL_ATTACHMENT, 
            GL_RENDERBUFFER, _rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Framebuffer not complete");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Create screen VAO
    glGenVertexArrays(1, &_screenVAO);
    glBindVertexArray(_screenVAO);

    const float square[] = {
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), &square, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

Window::~Window() {
}


void Window::_keyboardEvent(float delta) {
    if (glfwGetKey(_id, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(_id, true);
    }
    _camera.keyboardCallback(_id, delta);
}



void Window::_drawScene() {
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 matrix = _camera.matrix(false);
    _skybox->draw(matrix);

    glEnable(GL_DEPTH_TEST);
    _objectShader->use();
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _skybox->id());

    auto projection = _camera.matrix();
    _objectShader->setMat4("projection", projection);

    auto cameraPosition = _camera.position();
    _objectShader->setVec3("cameraPosition", cameraPosition);

    for (Model& model : _models) {
        model.draw(*_objectShader);
    }
}

void Window::_drawScreen() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    _screenShader->use();
    unsigned int screenTexture = _screenShader->uniformLocation("screenTexture");
    glUniform1i(screenTexture, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _screenTexture);
    glBindVertexArray(_screenVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Window::run() {

    float lastTime = glfwGetTime();


    while (!glfwWindowShouldClose(_id)) {
        float currentTime = glfwGetTime();
        float delta = currentTime - lastTime; 
        _keyboardEvent(delta);

        _drawScene();
        _drawScreen();

        glfwPollEvents();
        glfwSwapBuffers(_id);
    }
}

GLFWwindow* Window::id() {
    return _id;
}
