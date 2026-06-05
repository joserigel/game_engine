#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <assimp/scene.h>
#include <unordered_map>
#include <vector>

#include "mesh.hpp"
#include "texture.hpp"

using namespace std;

#define NUM_SUPPORTED_TEXTURE_TYPES 4

const aiTextureType supportedTextureTypes[NUM_SUPPORTED_TEXTURE_TYPES] = {
    aiTextureType_DIFFUSE,
    aiTextureType_SPECULAR,
    aiTextureType_NORMALS,
    aiTextureType_HEIGHT
};

class Model {
    private:
        vector<Mesh> meshes_;
        unordered_map<aiTextureType, Texture> textures_;
        glm::vec3 position_;

        void processNode_(aiNode* node, const aiScene* scene, string& directory);
        void processTexture_(aiMesh* mesh, const aiScene* scene, string& directory);
        Model();
    public:
        Model(const char* path);
        glm::vec3 position();
        glm::vec3 setPosition(glm::vec3 position);
        void draw(Shader& shader);

        void setTexture(aiTextureType type, const char* path);

        /**
         * Basic Plane Object Generator
         * @param position Position of the plane
         * @return Basic plane model object
         */
        static Model Plane(glm::vec3 position);
};

struct Object {
    Model* model;
    Shader* shader;
    glm::vec3 position;
};

#endif
