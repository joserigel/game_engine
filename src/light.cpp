#include "light.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include <glm/gtc/matrix_transform.hpp>

DirectionalLight::DirectionalLight(glm::vec3 direction) {
    direction_ = glm::normalize(direction);

    // Configure texture
    glGenTextures(1, &shadowTexture_);
    glBindTexture(GL_TEXTURE_2D, shadowTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
            DEFAULT_SHADOW_WIDTH, DEFAULT_SHADOW_HEIGHT, 0,
            GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  

    // Configure framebuffer
    glGenFramebuffers(1, &fbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_2D, shadowTexture_, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


unsigned int DirectionalLight::shadowTexture() {
    return shadowTexture_;
}

unsigned int DirectionalLight::fbo() {
    return fbo_;
}

glm::vec3 DirectionalLight::direction() {
    return direction_;
}

glm::mat4 DirectionalLight::matrix() {
    glm::mat4 ortho = glm::ortho(-3.0f, 3.0f, -3.0f, 3.0f, 0.1f, 5.0f);
    glm::mat4 view = glm::lookAt(
            -direction_,
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
    return ortho * view;
}
