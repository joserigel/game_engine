#ifndef __COMPONENTS_COLLIDER_HPP__
#define __COMPONENTS_COLLIDER_HPP__

#include "gameobject.hpp"

#include <glm/glm.hpp>

using namespace std;

namespace Collider {
    struct Contact {
        glm::vec3 point;
        glm::vec3 normal;
        float penetration;
    };

    bool getContact(
        GameObject& a, GameObject& b, Contact& contact);

}

#endif
