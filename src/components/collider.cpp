#include "collider.hpp"

using namespace Collider;

bool pointFaceContact(glm::vec3& point, GameObject& box, Contact& contact) {
    auto bLocal = glm::translate(glm::mat4_cast(box.rotation), box.position);
    auto relative = bLocal * glm::vec4(point, 1.0f);

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

float distanceBetweenEdges(
    glm::vec3& a, glm::vec3& b,
    glm::vec3& c, glm::vec3& d
) {
}

bool edgeEdgeContact(
    glm::vec3& a, glm::vec3& b,
    GameObject& box, Contact& contact) {
}

bool Collider::getContact(GameObject& a, GameObject& b, Contact& contact){
    // Scale, Rotation and Translation
    auto aLocal = glm::mat4_cast(a.rotation) 
        * glm::scale(glm::mat4(1.0f), a.size);
    aLocal = glm::translate(aLocal, a.position);

    const glm::vec3 vertices[] {
        glm::vec3(0.f, 0.f, 0.f),
    };

    bool collided = false;
    for (int i = 0; i < 1; i++) {
        Contact c;
        auto p = glm::vec3(aLocal * glm::vec4(vertices[0], 1.f));
        if (pointFaceContact(p, b, c)) {
            contact = c;
            return true;
        }

    }


    return false;
}
