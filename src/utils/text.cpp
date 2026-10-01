#include "text.hpp"

#include <GL/glew.h>

Text::Text(const char* path) :
    fontBitmap_(path), fontShader_("../shaders/text.vert", "../shaders/text.frag")
{
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    const float square[] = {
        0.0f,  0.0f,   0.0f, 0.0f,
        0.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
    };

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), &square, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Text::draw(std::string text, float size, glm::vec2 pos) {
    fontShader_.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fontBitmap_.id());
    fontShader_.setInt("fontBitmap", 0);

    for(auto c = text.begin(); c != text.end(); c++) {
        auto fontOffset = glm::vec2(0.0f, 0.0f);
        auto fontScale = glm::vec2(CHARACTER_COUNT_X, CHARACTER_COUNT_Y);
        fontShader_.setVec2("fontSize", fontOffset);
        fontShader_.setVec2("fontScale", fontScale);
    }


    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

