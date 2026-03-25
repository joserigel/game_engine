#include "mesh.hpp"

#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>

Mesh::Mesh(aiMesh* mesh, const aiScene* scene) {
    vector<Vertex> vertices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        vertex.position = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
            );
        vertex.normal = glm::vec3(
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z
            );
        
        // Technically up to AI_MAX_NUMBER_OF_TEXTURECOORDS is allowed, 
        // but we only look at 1
        if (mesh->mTextureCoords[0]) {
            vertex.textureCoords = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
                );
        } else {
            vertex.textureCoords = glm::vec2(0, 0);
        }
        vertices.push_back(vertex);
    }

    vector<unsigned int> indices;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    indicesCount_ = indices.size();

    // Generate Vertex Array
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // Generate Vertex Buffer Object
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(
        GL_ARRAY_BUFFER, 
        vertices.size() * sizeof(Vertex), 
        vertices.data(), 
        GL_STATIC_DRAW);
    
    // Generate Element Buffer Object
    glGenBuffers(1, &ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int),
        indices.data(),
        GL_STATIC_DRAW);

    // Set layouts
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 
        sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, 
        sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, 
        sizeof(Vertex), (void*)offsetof(Vertex, textureCoords));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Mesh::draw(Shader& shader) {
    glBindVertexArray(vao_);
    glDrawElements(
        GL_TRIANGLES, 
        indicesCount_,
        GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
