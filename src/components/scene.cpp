#include "scene.hpp"

#include "collider.hpp"
#include <glm/gtx/string_cast.hpp>

Scene::Scene()
    : camera_(),
        shader_("../shaders/debug.vert", "../shaders/debug.frag")
{
    objects_.push_back(GameObject::cube());
    objects_[0].size = glm::vec3(0.1f, 0.1f, 0.1f);
    objects_.push_back(GameObject::cube());
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
    }
}

void Scene::mouseCallback(GLFWwindow* id) {
    camera_.mouseCallback(id);
}

string Scene::debugText() {
    auto contact = Collider::getContact(objects_[0], objects_[1]);
    return "Point: " + glm::to_string(contact.point) + "\n" +
        "Normal: " + glm::to_string(contact.normal) + "\n" + 
        "Penetration: " + std::to_string(contact.penetration) + "\n"
        "Position : " +  glm::to_string(objects_[0].position);
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
}
