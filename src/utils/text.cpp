#include "text.hpp"

#include <GL/glew.h>
#include <vector>

std::pair<int, int> charToOffset(char c) {
    const std::vector<std::string> rows = {
        " ~`!@#$%^&*()",
        "_-+,.?/\\|:;'\"",
        "0123456789abc",
        "defghijklmnop",
        "qrstuvwxyzABC",
        "DEFGHIJKLMNOP",
        "QRSTUVWXYZ"
    };

    for (int y = 0; y < rows.size(); ++y)
    {
        for (int x = 0; x < rows[y].size(); ++x)
        {
            if (rows[y][x] == c) {
                return {x, CHARACTER_COUNT_Y - y - 1};
            }
        }
    }
    return {0, 0};
}

void Text::adjustAspectRatio(float ratio) {
    fontShader_.use();
    glm::mat2 mat(
            1.0f, 0.0f,
            0.0f, ratio);
    fontShader_.setMat2("ratio", mat);
}

Text::Text(const char* path) :
    fontBitmap_(path), fontShader_("../shaders/text.vert", "../shaders/text.frag")
{
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    const float square[] = {
        0.f,  0.f,  
        0.f / CHARACTER_COUNT_X, 1.f / CHARACTER_COUNT_Y, // top left
        0.f, -CHARACTER_HEIGHT,  
        0.f / CHARACTER_COUNT_X, 0.f / CHARACTER_COUNT_Y, // bottom left
        CHARACTER_WIDTH, -CHARACTER_HEIGHT,  
        1.f / CHARACTER_COUNT_X, 0.f / CHARACTER_COUNT_Y, // bottom right

        0.f,  0.f,  
        0.f / CHARACTER_COUNT_X, 1.f / CHARACTER_COUNT_Y, // top left
        CHARACTER_WIDTH, -CHARACTER_HEIGHT,  
        1.f / CHARACTER_COUNT_X, 0.f / CHARACTER_COUNT_Y, // bottom right
        CHARACTER_WIDTH,  0.f,  
        1.f / CHARACTER_COUNT_X, 1.f / CHARACTER_COUNT_Y // top right
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

    auto sizeMatrix = glm::mat2(
            1.0f, 0.0f,
            0.0f, 1.0f
            ) * size;
    
    fontShader_.setMat2("size", sizeMatrix);
    fontShader_.setVec2("position", pos);

    float physicalWidth = CHARACTER_WIDTH * size;
    float physicalHeight = CHARACTER_HEIGHT * size;
    float uvWidth = 1.0f / CHARACTER_COUNT_X;
    float uvHeight = 1.0f / CHARACTER_COUNT_Y;

    int i = 0;
    int breakLineCount = 0;

    glBindVertexArray(vao_);
    for(auto c = text.begin(); c != text.end(); c++) {
        if (*c == '\n') {
            breakLineCount++;
            i = 0;
            continue;
        }

        auto [u, v] = charToOffset(*c);
        auto fontOffset = glm::vec2(uvWidth * u, uvHeight * v);
        fontShader_.setVec2("fontOffset", fontOffset);

        auto position = pos + glm::vec2(i * physicalWidth, -breakLineCount * physicalHeight);
        fontShader_.setVec2("position", position);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        i++;
    }
}

