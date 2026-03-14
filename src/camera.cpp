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

Camera::Camera(float fov, int width, int height) : _fov(fov) {
    _aspectRatio = (float)width/(float)height;
    _projection = glm::perspective(
            glm::radians(fov), _aspectRatio, 0.1f, 100.0f);
    _view = glm::lookAt(
            _position, 
            _position + front(_pitch, _yaw), 
            glm::vec3(0, 1.0f, 0));
}


void Camera::setAspectRatio(int width, int height) {
    _aspectRatio = (float)width/(float)height;
    _projection = glm::perspective(
            glm::radians(_fov), _aspectRatio, 0.1f, 100.0f);
}

void Camera::mouseCallback(double xPos, double yPos) {
    if (_firstMouse) {
        _lastX = xPos;
        _lastY = yPos;
        _firstMouse = false;
    }

    float xDelta = xPos - _lastX;
    float yDelta = yPos - _lastY;
    _lastX = xPos;
    _lastY = yPos;

    _pitch -= yDelta * _lookSensitivity;
    _yaw -= xDelta * _lookSensitivity;
    _pitch = glm::clamp(_pitch, -89.0f, 89.0f);

    _view = glm::lookAt(
            _position,
            _position + front(_pitch, _yaw),
            glm::vec3(0, 1.0f, 0));
}

void Camera::keyboardCallback(GLFWwindow* window, float deltaTime) {
    glm::vec3 planarFront = front(_pitch, _yaw);
    planarFront.y = 0.0f;
    planarFront = glm::normalize(planarFront);
    glm::vec3 up = glm::vec3(0, 1.0f, 0);
    glm::vec3 right = glm::normalize(glm::cross(planarFront, up));
    
    if (glfwGetKey(window, GLFW_KEY_W)) {
        _position += planarFront * _moveSensitivity * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_S)) {
        _position -= planarFront * _moveSensitivity * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_D)) {
        _position += right * _moveSensitivity * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_A)) {
        _position -= right * _moveSensitivity * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE)) {
        _position += up * _moveSensitivity * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
        _position -= up * _moveSensitivity * deltaTime;
    }
    _view = glm::lookAt(
        _position, 
        _position + front(_pitch, _yaw),
        up);
}

glm::mat4 Camera::matrix() {
    return _projection * _view;
}
