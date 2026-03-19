#include "shader.hpp"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

#include <fstream>

using namespace std;

void Shader::use() {
    glUseProgram(_program);
}

unsigned int Shader::uniformLocation(const char* name) {
    return glGetUniformLocation(_program, name);
}


void Shader::setMat3(const char* name, glm::mat3& value) {
    unsigned int location = glGetUniformLocation(_program, name);
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat4(const char* name, glm::mat4& value) {
    unsigned int location = glGetUniformLocation(_program, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(const char* name, glm::vec3& value) {
    unsigned int location = glGetUniformLocation(_program, name);
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void Shader::setVec4(const char* name, glm::vec4& value) {
    unsigned int location = glGetUniformLocation(_program, name);
    glUniform4fv(location, 1, glm::value_ptr(value));
}

void Shader::setInt(const char* name, int value) {
    unsigned int location = glGetUniformLocation(_program, name);
    glUniform1i(location, value);
}

void Shader::setFloat(const char* name, float value) {
    unsigned int location = glGetUniformLocation(_program, name);
    glUniform1f(location, value);
}

Shader::Shader(
    const char* vertexPath,
    const char* fragmentPath
) {
    ifstream vertexFile(vertexPath);
    if (!vertexFile) {
        throw runtime_error(
            "file_read_error: " + string(vertexPath));
    }
    
    std::string line, vertexSource;
    while (std::getline(vertexFile, line)) {
        vertexSource += line + "\n";
    }
    const char* vertexSourceChar = vertexSource.c_str();
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(
        vertexShader, 1, &vertexSourceChar, nullptr);
    glCompileShader(vertexShader);
    int status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (!status) {
        char infoLog[1024];
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), 
            nullptr, infoLog);
        throw runtime_error("File Error " + string(vertexPath) + ": "+string(infoLog));
    }


    ifstream fragmentFile(fragmentPath);
    if (!fragmentFile) {
        throw runtime_error(
            "file_read_error: " + string(fragmentPath));
    }
    std::string fragmentSource;
    while (std::getline(fragmentFile, line)) {
        fragmentSource += line + "\n";
    }
    const char* fragmentSourceChar = fragmentSource.c_str();
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSourceChar, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (!status) {
        char infoLog[1024];
        glGetShaderInfoLog(fragmentShader, sizeof(infoLog),
            nullptr, infoLog);
        throw runtime_error("File Error " + string(fragmentPath) + ": "+string(infoLog));
    }

    _program = glCreateProgram();
    glAttachShader(_program, vertexShader);
    glAttachShader(_program, fragmentShader);
    glLinkProgram(_program);
    glGetProgramiv(_program, GL_LINK_STATUS, &status);
    if (!status) {
        char infoLog[1024];
        glGetProgramInfoLog(_program, sizeof(infoLog), nullptr, infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
