#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

#include <assimp/scene.h>

class Texture {
    private:
        unsigned int id_;
        aiTextureType type_;
    public:
        Texture(const char* path);
        unsigned int id();
};

#endif
