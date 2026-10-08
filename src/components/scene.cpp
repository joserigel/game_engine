#include "scene.hpp"

#include "collider.hpp"
#include <glm/gtx/string_cast.hpp>

Scene::Scene()
    : camera_(),
        shader_("../shaders/debug.vert", "../shaders/debug.frag")
{
    objects_.push_back(GameObject::cube());
    objects_.push_back(GameObject::cube());

    objects_[0].position += glm::vec3(1.0f);
    objects_[0].size = glm::vec3(0.5f);
}

void Scene::updateAspectRatio(int width, int height) {
    camera_.setAspectRatio(width, height);
}

void Scene::keyboardCallback(GLFWwindow* id, float delta) {
    camera_.keyboardCallback(id, delta);
    if (glfwGetKey(id, GLFW_KEY_RIGHT)) {
        objects_[0].position.x += 0.01f;
    } else if (glfwGetKey(id, GLFW_KEY_LEFT)) {
        objects_[0].position.x -= 0.01f;
    } else if (glfwGetKey(id, GLFW_KEY_UP)) {
        objects_[0].position.z += 0.01f;
    } else if (glfwGetKey(id, GLFW_KEY_DOWN)) {
        objects_[0].position.z -= 0.01f;
    } else if (glfwGetKey(id, GLFW_KEY_PAGE_UP)) {
        objects_[0].position.y += 0.01f;
    } else if (glfwGetKey(id, GLFW_KEY_PAGE_DOWN)) {
        objects_[0].position.y -= 0.01f;
    } else if (glfwGetKey(id, GLFW_KEY_MINUS)) {
        auto delta = glm::angleAxis(
                glm::radians(-5.f),
                glm::vec3(0.f, 1.0f, 0.f)
                );
        objects_[0].rotation = delta * objects_[0].rotation;
    } else if (glfwGetKey(id, GLFW_KEY_EQUAL)) {
        auto delta = glm::angleAxis(
                glm::radians(5.f),
                glm::vec3(0.f, 1.f, 0.f)
                );
        objects_[0].rotation = delta * objects_[0].rotation;
    } else if (glfwGetKey(id, GLFW_KEY_LEFT_BRACKET)) {
        auto delta = glm::angleAxis(
                glm::radians(-5.f),
                glm::vec3(1.f, 0.f, 0.f)
                );
        objects_[0].rotation = delta * objects_[0].rotation;
    } else if (glfwGetKey(id, GLFW_KEY_RIGHT_BRACKET)) {
        auto delta = glm::angleAxis(
                glm::radians(5.f),
                glm::vec3(1.f, .0f, 0.f)
                );
        objects_[0].rotation = delta * objects_[0].rotation;
    } else if (glfwGetKey(id, GLFW_KEY_SEMICOLON)) {
        auto delta = glm::angleAxis(
                glm::radians(-5.f),
                glm::vec3(0.f, .0f, 1.f)
                );
        objects_[0].rotation = delta * objects_[0].rotation;
    } else if (glfwGetKey(id, GLFW_KEY_APOSTROPHE)) {
        auto delta = glm::angleAxis(
                glm::radians(5.f),
                glm::vec3(0.f, .0f, 1.f)
                );
        objects_[0].rotation = delta * objects_[0].rotation;
    }
}

void Scene::mouseCallback(GLFWwindow* id) {
    if (glfwGetKey(id, GLFW_KEY_G)) {
        glfwSetInputMode(id, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        camera_.mouseCallback(id);
    } else {
        glfwSetInputMode(id, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

string Scene::debugText() {
    Collider::Contact contact;
    auto collides = Collider::getContact(objects_[0], objects_[1], contact);
    if (collides) {
        return "Point: " + glm::to_string(contact.point) + "\n" +
            "Normal: " + glm::to_string(contact.normal) + "\n" + 
            "Penetration: " + std::to_string(contact.penetration) + "\n"
            "Position : " +  glm::to_string(objects_[0].position);
    } else {
        return "No collision";
    }
}

void Scene::fixCollision() {
    Collider::Contact contact;
    auto collides = Collider::getContact(objects_[0], objects_[1], contact);
    if (collides) {
        objects_[0].position += contact.normal * contact.penetration;
    }
}

void tick() {
}

void Scene::draw() {
    shader_.use();
    auto projection = camera_.matrix();
    shader_.setMat4("projection", projection);
    for (auto object : objects_) {
        object.draw(shader_);
    }

    Collider::Contact contact;
    bool collides = Collider::getContact(
            objects_[0],
            objects_[1],
            contact);
    if (collides) {
        auto mat = camera_.matrix();
        auto a = contact.point;
        auto b = contact.point + (contact.normal*contact.penetration);
        debugCube_.draw(mat, a, glm::vec3(1.0));
        debugCube_.drawLine(projection, a, b, glm::vec3(1.0));
        debugCube_.draw(mat, b, glm::vec3(1.0, 0.0, 0.0));
    }

    debugCube_.drawLine(projection, 
            glm::vec3(0.0f), 
            glm::vec3(100.0f, 0.f, 0.f), glm::vec3(1.0f, 0.0f, 0.0f));
    debugCube_.drawLine(projection, 
            glm::vec3(0.0f), 
            glm::vec3(0.0f, 100.f, 0.f), glm::vec3(0.0f, 1.0f, 0.0f));
    debugCube_.drawLine(projection, 
            glm::vec3(0.0f), 
            glm::vec3(0.0f, 0.f, 100.f), glm::vec3(0.0f, 0.0f, 1.0f));

}
