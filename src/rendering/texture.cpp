#include "texture.hpp"

#include <GL/glew.h>
#include <GL/gl.h>

#include "../utils/stb_image.h"
#include <glm/glm.hpp>
#include <string>
#include <stdexcept>

using namespace std;

unsigned int Texture::id() {
    return id_;
}

Texture::Texture(const char* path) {

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

    if (!data) {
        throw runtime_error(string(path) + " not found!");
    }

    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);

    GLenum format;
    switch(nrChannels) {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            throw runtime_error("Unknown format:" + string(path));
    }
    // What tf is this??? man cuz it's not divisible by 4
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, 
        format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}
