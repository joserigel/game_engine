#include "model.hpp"

#include <stdexcept>
#include <GL/glew.h>
#include <GL/gl.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb_image.h"

using namespace std;

Texture loadTextureFromFile(const char* path) { 
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

    if (!data) {
        throw std::runtime_error(std::string(path) + " not found!");
    }

    Texture texture;
    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

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
            throw std::runtime_error("Unknown format:" + string(path));
    }
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, 
        format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return texture;
}

void Model::processTexture_(aiMesh* mesh, const aiScene* scene, string& directory) 
{
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    

    for (int i = 0; i < NUM_SUPPORTED_TEXTURE_TYPES; i++) {
        aiTextureType type = supportedTextureTypes[i];
        if (material->GetTextureCount(type) > 0 &&
                textures_.find(type) == textures_.end()) {
            aiString filename;
            material->GetTexture(type, 0, &filename);
            string path = directory + "/" + filename.C_Str();
            textures_[type] = loadTextureFromFile(path.c_str());
        }
    }
}

void Model::processNode_(
    aiNode* node, const aiScene* scene, string& directory
) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes_.emplace_back(mesh, scene);
        processTexture_(mesh, scene, directory);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode_(node->mChildren[i], scene, directory);
    }
}

Model::Model(const char* path) {
    std::string directory(path);
    directory = directory.substr(0, directory.find_last_of("/"));

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
            path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || 
        scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        throw std::runtime_error(importer.GetErrorString());
        return;
    }

    processNode_(scene->mRootNode, scene, directory);
}


void Model::draw(Shader& shader) {
    shader.use();

    for (int i = 0; i < NUM_SUPPORTED_TEXTURE_TYPES; i++) {
        aiTextureType type = supportedTextureTypes[i];

        if (textures_.find(type) != textures_.end()) {
            string typeStr = aiTextureTypeToString(type);
            std::transform(
                    typeStr.begin(), 
                    typeStr.end(), typeStr.begin(), ::tolower);
            Texture texture = textures_[type];
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, texture.id);
            shader.setInt(
                string(typeStr + "_texture").c_str(),
                i);
        }
    }

    for (Mesh& mesh : meshes_) {
        mesh.draw(shader);
    }
}
