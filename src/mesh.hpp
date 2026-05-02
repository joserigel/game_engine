#ifndef __MESH_HPP__
#define __MESH_HPP__

#include <vector>

#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>

#include "shader.hpp"

using namespace std;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

class Mesh {
    private:
        unsigned int vao_;
        unsigned int vbo_;
        unsigned int ebo_;
        unsigned int indicesCount_;

    public:
        Mesh(vector<Vertex>& vertices, vector<unsigned int>& indices);
        Mesh(aiMesh* mesh, const aiScene* scene);
        void draw(Shader& shader);
};

#endif
