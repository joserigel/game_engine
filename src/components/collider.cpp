#include "collider.hpp"

#include <glm/gtx/string_cast.hpp>
#include <iostream>

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

float distanceBetweenPointAndEdge(
    glm::vec3& p,
    glm::vec3& a, glm::vec3& b
) {
    glm::vec3 ab = b - a;
    float t = glm::dot(p - a, ab) / glm::dot(ab, ab);

    if (t < 0.f) {
        t = 0.f;
    }
    if (t > 1.f) {
        t = 1.f;
    }
    glm::vec3 d = a + t * ab;
    return glm::length(d - p);
}


float distanceBetweenEdges(
    glm::vec3& p1, glm::vec3& q1,
    glm::vec3& p2, glm::vec3& q2,
    float& s, float &t
) {
    glm::vec3 d1 = q1 - p1;
    glm::vec3 d2 = q2 - p1;
    glm::vec3 r = p1 - p2;
    float a = glm::dot(d1, d1);
    float b = glm::dot(d1, d2);
    float c = glm::dot(d1, r);
    float e = glm::dot(d2, d2);
    float f = glm::dot(d2, r);

    float denom = a*e-b*b;

    if (denom != 0.0f) {
        s = glm::clamp((b*f - c*e) / denom, 0.0f, 1.0f);
    } else {
        s = 0.0f;
    }
    
    t = (b*s + f) / e;
    if (t < 0.f) {
        t = 0.f;
        s = glm::clamp(-c / a, 0.f, 1.f);
    } else if (t > 1.f) {
        t = 1.f;
        s = glm::clamp((b - c) / a, 0.f, 1.f);
    }
    glm::vec3 c1 = p1 + d1 * s;
    glm::vec3 c2 = p2 + d2 * t;
    return glm::length(c1 - c2);
}

bool edgeEdgeContact(
    GameObject& a, GameObject& b,
    Contact& contact) {

    const glm::vec3 edges[] {
        // Bottom face
        glm::vec3(-.5f, -.5f, -.5f), glm::vec3( .5f, -.5f, -.5f),
        glm::vec3( .5f, -.5f, -.5f), glm::vec3( .5f, -.5f,  .5f),
        glm::vec3( .5f, -.5f,  .5f), glm::vec3(-.5f, -.5f,  .5f),
        glm::vec3(-.5f, -.5f,  .5f), glm::vec3(-.5f, -.5f, -.5f),

        // Top face
        glm::vec3(-.5f,  .5f, -.5f), glm::vec3( .5f,  .5f, -.5f),
        glm::vec3( .5f,  .5f, -.5f), glm::vec3( .5f,  .5f,  .5f),
        glm::vec3( .5f,  .5f,  .5f), glm::vec3(-.5f,  .5f,  .5f),
        glm::vec3(-.5f,  .5f,  .5f), glm::vec3(-.5f,  .5f, -.5f),

        // Vertical edges
        glm::vec3(-.5f, -.5f, -.5f), glm::vec3(-.5f,  .5f, -.5f),
        glm::vec3( .5f, -.5f, -.5f), glm::vec3( .5f,  .5f, -.5f),
        glm::vec3( .5f, -.5f,  .5f), glm::vec3( .5f,  .5f,  .5f),
        glm::vec3(-.5f, -.5f,  .5f), glm::vec3(-.5f,  .5f,  .5f),
    };

    auto aLocal = glm::translate(glm::mat4(1.f), a.position)
        * glm::mat4_cast(a.rotation)
        * glm::scale(glm::mat4(1.f), a.size);
    auto bLocal = glm::translate(glm::mat4(1.f), b.position)
        * glm::mat4_cast(b.rotation)
        * glm::scale(glm::mat4(1.f), b.size);

    bool overallCollided = false;
    for (int i = 0; i < 24; i+=2) {
        bool collided = false;
        Collider::Contact c;
        for (int j = 0; j < 24; j+=2) {
            float s, t;
            glm::vec3 p1 = glm::vec3(aLocal * glm::vec4(edges[i], 1.f));
            glm::vec3 q1 = glm::vec3(aLocal * glm::vec4(edges[i+1], 1.f));
            glm::vec3 p2 = glm::vec3(bLocal * glm::vec4(edges[j], 1.f));
            glm::vec3 q2 = glm::vec3(bLocal * glm::vec4(edges[j+1], 1.f));
            float d = distanceBetweenEdges(
                p1, q1, p2, q2,
                s, t);

            glm::vec3 closestA = p1 + (q1 - p1) * s;
            glm::vec3 closestB = p2 + (q2 - p2) * t;
            if (glm::length(closestA - b.position) < glm::length(closestB - b.position) 
                && (!collided || c.penetration > d))
            {
                c.penetration = d;
                c.normal = glm::normalize(closestB - closestA);
                c.point = closestA;
                collided = true;
            }
        }
        if (collided && (!overallCollided || contact.penetration < c.penetration)) {
            contact = c;
            overallCollided = true;
        }
    }

    return overallCollided;
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
        if (penetrated) {
            std::cout << glm::to_string(p) << ":" << c.penetration << std::endl;
        }
        if (penetrated && (!collided || contact.penetration < c.penetration)) {
            contact = c;
            collided = true;
        }
    }
    std::cout << "\n" << std::endl;
    
    Collider::Contact edgeContact;
    bool edgePenetration = edgeEdgeContact(a, b, edgeContact);
    if (edgePenetration && !collided) {
        contact = edgeContact;
        return true;
    } else if (edgePenetration && collided 
        && edgeContact.penetration > contact.penetration
    ) {
        contact = edgeContact;
        return true;
    }


    return collided;
}
