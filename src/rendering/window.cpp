#include "window.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>

void Window::sizeCallback_(GLFWwindow* window, int width, int height) {
    Window* windowObject = static_cast<Window*>(glfwGetWindowUserPointer(window));
    glViewport(0, 0, width, height);

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

Window::Window() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,
            GLFW_OPENGL_CORE_PROFILE);

    id_ = glfwCreateWindow(
            WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT, 
            "GameEngine", nullptr, nullptr);
    glfwMakeContextCurrent(id_);
    glfwSetInputMode(id_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GLenum error_code = glewInit();
    if (error_code) {
        throw std::runtime_error("Cannot initialize glew");
    }


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(1.0f);
    style.FontScaleDpi = 1.0f;

    ImGui_ImplGlfw_InitForOpenGL(id_, true);

    const char* glsl_version = nullptr;
    ImGui_ImplOpenGL3_Init(glsl_version);


    // Set cursor callback
    glViewport(0, 0, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT);
    glfwSetWindowUserPointer(id_, (void*)this);
    glfwSetFramebufferSizeCallback(id_, Window::sizeCallback_);
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


    screenShader_ = std::make_shared<Shader>(
            "../shaders/screen.vert", "../shaders/screen.frag");

    glBindVertexArray(0);
}


Window::~Window() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    glfwDestroyWindow(id_);
    glfwTerminate();
}

void Window::keyboardEvent_(float delta) {
    if (glfwGetKey(id_, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(id_, true);
    } else if (glfwGetKey(id_, GLFW_KEY_F)) {
        glfwSetInputMode(id_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else if (glfwGetKey(id_, GLFW_KEY_H)) {
        glfwSetInputMode(id_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}


void Window::drawScreen_() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    screenShader_->use();
    screenShader_->setInt("screentexture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screenTexture_);
    glBindVertexArray(screenVAO_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Window::run() {
    float lastTime = glfwGetTime();

    int i = 0;
    while (!glfwWindowShouldClose(id_)) {
        glfwPollEvents();
        float currentTime = glfwGetTime();
        float delta = currentTime - lastTime; 

        keyboardEvent_(delta);


        glViewport(0, 0, width_, height_);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer_);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawScreen_();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Hello World");
        std::string text = "Hello test: " + std::to_string(i);
        ImGui::Text("%s", text.c_str());
        if(ImGui::Button("Button Test"))
            i++;

        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(id_, &display_w, &display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(id_);
        lastTime = currentTime;
    }
}

GLFWwindow* Window::id() {
    return id_;
}
