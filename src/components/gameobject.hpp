#ifndef __COMPONENTS_GAMEOBJECT_HPP__
#define __COMPONENTS_GAMEOBJECT_HPP__

#include "../rendering/mesh.hpp"
#include "../rendering/texture.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace std;


class GameObject {
    private:
        Mesh mesh_;
        Texture texture_;


        GameObject(Mesh mesh, Texture texture);
    public:
        void draw(Shader& shader);


        static GameObject cube();

        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 velocity = glm::vec3(0.0f);
        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 size = glm::vec3(1.0f);

        float mass = 1.0f;

};

#endif
