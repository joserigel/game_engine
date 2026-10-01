#ifndef __COMPONENTS_GAMEOBJECT_HPP__
#define __COMPONENTS_GAMEOBJECT_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

class GameObject {
    private:
        glm::vec3 position_;
        glm::quat rotation_;
        std::vector<GameObject> children_;

    public:
};

#endif
