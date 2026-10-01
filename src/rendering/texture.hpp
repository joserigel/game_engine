#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

#include <assimp/scene.h>

class Texture {
    private:
        unsigned int id_ = 0;
        aiTextureType type_ = aiTextureType::aiTextureType_DIFFUSE;
    public:
        Texture();
        Texture(const char* path);
        unsigned int id();
};

#endif
