#ifndef __MESH_HPP__
#define __MESH_HPP__

#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>

#include "shader.hpp"

using namespace std;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoords;
};

struct Texture {
    unsigned int id;
    aiTextureType type;
    std::string path;
};

class Mesh {
    private:
        unsigned int vao_;
        unsigned int vbo_;
        unsigned int ebo_;
        unsigned int indicesCount_;

    public:
        Mesh(aiMesh* mesh, const aiScene* scene);
        void draw(Shader& shader);
};

#endif
