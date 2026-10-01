#ifndef __UTILS_TEXT_HPP__
#define __UTILS_TEXT_HPP__

#include <string>

#include "../rendering/texture.hpp"
#include "../rendering/shader.hpp"


#define CHARACTER_COUNT_X 13
#define CHARACTER_COUNT_Y 7
#define CHARACTER_WIDTH 68
#define CHARACTER_HEIGHT 83


class Text {
    private:
        Texture fontBitmap_;
        Shader fontShader_;

        unsigned int vao_;
        unsigned int vbo_;
        unsigned int ebo_;
    public:
        Text(const char* path);
        void draw(std::string text, float size, glm::vec2 pos);
};

#endif
