#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <assimp/scene.h>
#include <unordered_map>
#include <vector>

#include "mesh.hpp"

using namespace std;

#define NUM_SUPPORTED_TEXTURE_TYPES 2

const aiTextureType supportedTextureTypes[NUM_SUPPORTED_TEXTURE_TYPES] = {
    aiTextureType_DIFFUSE,
    aiTextureType_SPECULAR
};

class Model {
    private:
        vector<Mesh> meshes_;
        unordered_map<aiTextureType, Texture> textures_;

        void processNode_(aiNode* node, const aiScene* scene, string& directory);
        void processTexture_(aiMesh* mesh, const aiScene* scene, string& directory);
    public:
        Model(const char* path);
        void draw(Shader& shader);
};

#endif
