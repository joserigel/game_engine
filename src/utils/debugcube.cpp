#include "debugcube.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>

DebugCube::DebugCube() :
    shader_("../shaders/debugcube.vert", "../shaders/debugcube.frag")
{
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    float vertices[] = {
        // positions          
        -.5f,  .5f, -.5f,
        -.5f, -.5f, -.5f,
         .5f, -.5f, -.5f,
         .5f, -.5f, -.5f,
         .5f,  .5f, -.5f,
        -.5f,  .5f, -.5f,

        -.5f, -.5f,  .5f,
        -.5f, -.5f, -.5f,
        -.5f,  .5f, -.5f,
        -.5f,  .5f, -.5f,
        -.5f,  .5f,  .5f,
        -.5f, -.5f,  .5f,

         .5f, -.5f, -.5f,
         .5f, -.5f,  .5f,
         .5f,  .5f,  .5f,
         .5f,  .5f,  .5f,
         .5f,  .5f, -.5f,
         .5f, -.5f, -.5f,

        -.5f, -.5f,  .5f,
        -.5f,  .5f,  .5f,
         .5f,  .5f,  .5f,
         .5f,  .5f,  .5f,
         .5f, -.5f,  .5f,
        -.5f, -.5f,  .5f,

        -.5f,  .5f, -.5f,
         .5f,  .5f, -.5f,
         .5f,  .5f,  .5f,
         .5f,  .5f,  .5f,
        -.5f,  .5f,  .5f,
        -.5f,  .5f, -.5f,

        -.5f, -.5f, -.5f,
        -.5f, -.5f,  .5f,
         .5f, -.5f, -.5f,
         .5f, -.5f, -.5f,
        -.5f, -.5f,  .5f,
         .5f, -.5f,  .5f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices,
            GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
            3*sizeof(float), 0);
    glEnableVertexAttribArray(0);
}

void DebugCube::draw(glm::mat4& projection, glm::vec3& position, glm::vec3 color) {
    shader_.use();
    shader_.setMat4("projection", projection);
    glm::mat4 local = glm::translate(glm::mat4(1.f), position) 
        * glm::scale(glm::mat4(1.f), glm::vec3(DEBUGCUBE_SIZE));
    shader_.setMat4("local", local);
    shader_.setVec3("color", color);
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void DebugCube::drawLine(glm::mat4& projection, glm::vec3 a, glm::vec3 b, glm::vec3 color) {
    shader_.use();
    shader_.setMat4("projection", projection);
    auto ab = glm::normalize(b - a);
    auto length = glm::length(b - a);
    glm::quat q = glm::rotation(glm::vec3(0.0, 0.0, 1.0), ab);
    glm::mat4 local = 
        glm::translate(glm::mat4(1.f), a)
        * glm::mat4_cast(q)
        * glm::scale(glm::mat4(1.f), glm::vec3(DEBUGCUBE_SIZE, DEBUGCUBE_SIZE, glm::length(b-a)))
        * glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 0.0f, 0.5f));
    shader_.setMat4("local", local);
    shader_.setVec3("color", color);
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
