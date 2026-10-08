#include "collider.hpp"


using namespace Collider;

bool pointFaceContact(glm::vec3& point, GameObject& box, Contact& contact) {
    auto bLocal = glm::translate(glm::mat4(1.f), box.position)
        * glm::mat4_cast(box.rotation);
    auto relative = glm::inverse(bLocal) * glm::vec4(point, 1.0f);

    float minDepth = (box.size.x / 2) - abs(relative.x);
    if (minDepth < 0.f) {
        return false;
    }
    glm::vec3 normal = glm::vec3(1.f, 0.f, 0.f) * (relative.x < 0 ? -1.f : 1.f);

    float depth = (box.size.y / 2) - abs(relative.y);
    if (depth < 0.f) {
        return false;
    } else if (depth < minDepth) {
        minDepth = depth;
        normal = glm::vec3(0.f, 1.f, 0.f) * (relative.y < 0 ? -1.f : 1.f);
    }

    depth = (box.size.z / 2) - abs(relative.z);
    if (depth < 0.f) {
        return false;
    } else if (depth < minDepth) {
        minDepth = depth;
        normal = glm::vec3(0.f, 0.f, 1.f) * (relative.z < 0 ? -1.f : 1.f);
    }

    contact.normal = glm::vec3(glm::mat4_cast(box.rotation) * glm::vec4(normal, 1.0f));
    contact.point = point;
    contact.penetration = minDepth;
    
    return true;
}

bool Collider::getContact(GameObject& a, GameObject& b, Contact& contact){
    // Scale, Rotation and Translation
    auto aLocal = glm::translate(glm::mat4(1.f), a.position)
        * glm::mat4_cast(a.rotation) 
        * glm::scale(glm::mat4(1.f), a.size);

    const glm::vec3 vertices[] {
        glm::vec3(-.5f, -.5f, -.5f),
        glm::vec3(-.5f, -.5f,  .5f),
        glm::vec3(-.5f,  .5f, -.5f),
        glm::vec3(-.5f,  .5f,  .5f),
        glm::vec3( .5f, -.5f, -.5f),
        glm::vec3( .5f, -.5f,  .5f),
        glm::vec3( .5f,  .5f, -.5f),
        glm::vec3( .5f,  .5f,  .5f)
    };

    // Has collided check
    bool collided = false;
    for (int i = 0; i < 8; i++) {
        Contact c;
        auto p = glm::vec3(aLocal * glm::vec4(vertices[i], 1.f));
        
        // Current point collided check
        auto penetrated = pointFaceContact(p, b, c);
        if (penetrated && (!collided || contact.penetration < c.penetration)) {
            contact = c;
            collided = true;
        }
    }
    

    return collided;
}
