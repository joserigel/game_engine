#include "model.hpp"

#include <stdexcept>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

void Model::_processNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.emplace_back(mesh, scene);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        _processNode(node->mChildren[i], scene);
    }
}

Model::Model(const char* path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
            path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || 
        scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        throw std::runtime_error(importer.GetErrorString());
        return;
    }

    _processNode(scene->mRootNode, scene);
}

void Model::draw(Shader& shader) {
    shader.use();
    for (Mesh& mesh : _meshes) {
        mesh.draw(shader);
    }
}
