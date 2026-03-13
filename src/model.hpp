#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <assimp/scene.h>
#include <vector>

#include "mesh.hpp"

class Model {
    private:
        vector<Mesh> _meshes;

        void _processNode(aiNode* node, const aiScene* scene);
        void _processMesh(aiMesh* mesh);
    public:
        Model(const char* path);
        void draw(Shader& shader);
};

#endif
