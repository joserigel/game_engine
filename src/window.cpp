#include "window.hpp"

#include "model.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>

void Window::sizeCallback_(GLFWwindow* window, int width, int height) {
    Window* windowObject = static_cast<Window*>(glfwGetWindowUserPointer(window));
    glViewport(0, 0, width, height);
    windowObject->camera_.setAspectRatio(width, height);

    windowObject->width_ = width;
    windowObject->height_ = height;
    
    glBindTexture(GL_TEXTURE_2D, windowObject->screenTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0,
            GL_RGB, width, height,
            0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindRenderbuffer(GL_RENDERBUFFER, windowObject->rbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
            width, height);
}

void Window::cursorPosCallback_(GLFWwindow* window, double xpos, double ypos) {
    Window* windowObject = static_cast<Window*>(glfwGetWindowUserPointer(window));
    windowObject->camera_.mouseCallback(xpos, ypos);
}

Window::Window() : 
    camera_(WINDOW_DEFAULT_FOV, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT)
{
    id_ = glfwCreateWindow(
            WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT, 
            "GameEngine", nullptr, nullptr);
    glfwMakeContextCurrent(id_);
    glfwSetInputMode(id_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GLenum error_code = glewInit();
    if (error_code) {
        throw std::runtime_error("Cannot initialize glew");
    }

    light_ = make_unique<DirectionalLight>();

    objectShader_ = make_unique<Shader>("../shaders/basic.vert", 
            "../shaders/basic.frag");
    screenShader_ = make_unique<Shader>("../shaders/screen.vert", 
            "../shaders/screen.frag");
    shadowShader_ = make_unique<Shader>("../shaders/shadow.vert",
            "../shaders/shadow.frag");
    skybox_ = make_unique<CubeMap>(
        "../models/skybox/left.jpg",
        "../models/skybox/right.jpg",
        "../models/skybox/bottom.jpg",
        "../models/skybox/top.jpg",
        "../models/skybox/front.jpg",
        "../models/skybox/back.jpg"
        );
    objectShader_->use();
    unsigned int skyboxLoc = objectShader_->uniformLocation("skybox");
    glUniform1i(skyboxLoc, 0);

    // models_.emplace_back("../models/backpack/backpack.obj");
    models_.push_back(Model::Plane(glm::vec3(0.f,0.f,0.f)));
    models_[0].setTexture(aiTextureType_DIFFUSE, "../models/wood.png");
    models_[0].setTexture(aiTextureType_NORMALS, "../models/toy_box_normal.png");
    models_[0].setTexture(aiTextureType_HEIGHT, "../models/toy_box_disp.png");

    // Set cursor callback
    glViewport(0, 0, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT);
    glfwSetWindowUserPointer(id_, (void*)this);
    glfwSetFramebufferSizeCallback(id_, Window::sizeCallback_);
    glfwSetCursorPosCallback(id_, Window::cursorPosCallback_);
    glEnable(GL_DEPTH_TEST);

    // Create Frame buffer
    glGenFramebuffers(1, &frameBuffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer_);

    // Create screen texture
    glGenTextures(1, &screenTexture_);
    glBindTexture(GL_TEXTURE_2D, screenTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0,
            GL_RGB, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT,
            0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D, screenTexture_, 0);

    // Create render buffer for depth and stencil
    glGenRenderbuffers(1, &rbo_);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
            WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_WIDTH);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, 
            GL_DEPTH_STENCIL_ATTACHMENT, 
            GL_RENDERBUFFER, rbo_);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Framebuffer not complete");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Create screen VAO
    glGenVertexArrays(1, &screenVAO_);
    glBindVertexArray(screenVAO_);

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
}


Window::~Window() {
}

void Window::keyboardEvent_(float delta) {
    if (glfwGetKey(id_, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(id_, true);
    }
    camera_.keyboardCallback(id_, delta);
}

void Window::drawShadow_() {
    glBindFramebuffer(GL_FRAMEBUFFER, light_->fbo());
    glViewport(0, 0, DEFAULT_SHADOW_WIDTH, DEFAULT_SHADOW_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    shadowShader_->use();
    auto projection = light_->matrix();
    shadowShader_->setMat4("projection", projection);

    for (Model& model : models_) {
        model.draw(*shadowShader_);
    }
}

void Window::drawScene_() {
    glViewport(0, 0, width_, height_);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer_);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 matrix = camera_.matrix(false);
    skybox_->draw(matrix);

    glEnable(GL_DEPTH_TEST);

    objectShader_->use();
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, light_->shadowTexture());
    objectShader_->setInt("shadowMap", 4);

    auto projection = camera_.matrix();
    objectShader_->setMat4("projection", projection);

    auto lightMatrix = light_->matrix();
    objectShader_->setMat4("lightMatrix", lightMatrix);

    auto lightDir = light_->direction();
    objectShader_->setVec3("lightDir", lightDir);

    auto cameraPosition = camera_.position();
    objectShader_->setVec3("cameraPosition", cameraPosition);

    for (Model& model : models_) {
        model.draw(*objectShader_);
    }
}

void Window::drawScreen_() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    screenShader_->use();
    screenShader_->setInt("screentexture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screenTexture_);
    glBindVertexArray(screenVAO_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Window::run() {
    float lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(id_)) {
        float currentTime = glfwGetTime();
        float delta = currentTime - lastTime; 
        keyboardEvent_(delta);

        glm::vec3 dir = glm::vec3(sin(currentTime), 0, cos(currentTime));
        light_->setDirection(dir);
    
        drawShadow_();
        drawScene_();
        drawScreen_();

        glfwPollEvents();
        glfwSwapBuffers(id_);
    }
}

GLFWwindow* Window::id() {
    return id_;
}
