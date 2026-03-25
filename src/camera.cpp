#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

inline glm::vec3 front(float pitch, float yaw) {
    pitch = glm::radians(pitch);
    yaw = glm::radians(yaw);

    glm::vec3 res;
    res.x = sin(yaw) * cos(pitch);
    res.y = sin(pitch);
    res.z = cos(yaw) * cos(pitch);

    return res;
}

Camera::Camera(float fov, int width, int height) : fov_(fov) {
    aspectRatio_ = (float)width/(float)height;
    projection_ = glm::perspective(
            glm::radians(fov), aspectRatio_, 0.1f, 100.0f);
    view_ = glm::lookAt(
            position_, 
            position_ + front(pitch_, yaw_), 
            glm::vec3(0, 1.0f, 0));
}


void Camera::setAspectRatio(int width, int height) {
    aspectRatio_ = (float)width/(float)height;
    projection_ = glm::perspective(
            glm::radians(fov_), aspectRatio_, 0.1f, 100.0f);
}

void Camera::mouseCallback(double xPos, double yPos) {
    if (firstMouse_) {
        lastX_ = xPos;
        lastY_ = yPos;
        firstMouse_ = false;
    }

    float xDelta = xPos - lastX_;
    float yDelta = yPos - lastY_;
    lastX_ = xPos;
    lastY_ = yPos;

    pitch_ -= yDelta * lookSensitivity_;
    yaw_ -= xDelta * lookSensitivity_;
    pitch_ = glm::clamp(pitch_, -89.0f, 89.0f);

    view_ = glm::lookAt(
            position_,
            position_ + front(pitch_, yaw_),
            glm::vec3(0, 1.0f, 0));
}

void Camera::keyboardCallback(GLFWwindow* window, float deltaTime) {
    glm::vec3 planarFront = front(pitch_, yaw_);
    planarFront.y = 0.0f;
    planarFront = glm::normalize(planarFront);
    glm::vec3 up = glm::vec3(0, 1.0f, 0);
    glm::vec3 right = glm::normalize(glm::cross(planarFront, up));
    
    if (glfwGetKey(window, GLFW_KEY_W)) {
        position_ += planarFront * moveSensitivity_ * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_S)) {
        position_ -= planarFront * moveSensitivity_ * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_D)) {
        position_ += right * moveSensitivity_ * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_A)) {
        position_ -= right * moveSensitivity_ * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE)) {
        position_ += up * moveSensitivity_ * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
        position_ -= up * moveSensitivity_ * deltaTime;
    }
    view_ = glm::lookAt(
        position_, 
        position_ + front(pitch_, yaw_),
        up);
}

glm::vec3 Camera::position() {
    return position_;
}

glm::mat4 Camera::matrix(bool translation) {
    if (translation) {
        return projection_ * view_;
    } else {
        glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f), front(pitch_, yaw_), 
            glm::vec3(0.0f, 1.0f, 0.0f));
        return projection_ * view;
    }
}
