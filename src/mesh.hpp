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
        unsigned int _vao;
        unsigned int _vbo;
        unsigned int _ebo;
        unsigned int _indicesCount;

    public:
        Mesh(aiMesh* mesh, const aiScene* scene);
        void draw(Shader& shader);
};

#endif
