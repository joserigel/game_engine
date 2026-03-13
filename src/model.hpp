#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <assimp/scene.h>
#include <unordered_map>
#include <vector>

#include "mesh.hpp"

using namespace std;

class Model {
    private:
        vector<Mesh> _meshes;
        unordered_map<aiTextureType, Texture> _textures;

        void _processNode(aiNode* node, const aiScene* scene, string& directory);
        void _processTexture(aiMesh* mesh, const aiScene* scene, string& directory);
    public:
        Model(const char* path);
        void draw(Shader& shader);
};

#endif
