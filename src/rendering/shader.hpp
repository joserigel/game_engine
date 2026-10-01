#ifndef __RENDERING_SHADER_HPP__
#define __RENDERING_SHADER_HPP__

#include <glm/glm.hpp>

class Shader {
    private:
        unsigned int program_;
    public:
        void use();
        unsigned int uniformLocation(const char* name);

        void setMat2(const char* name, glm::mat2& value);
        void setMat3(const char* name, glm::mat3& value);
        void setMat4(const char* name, glm::mat4& value);
        void setVec2(const char* name, glm::vec2& value);
        void setVec3(const char* name, glm::vec3& value);
        void setVec4(const char* name, glm::vec4& value);
        void setInt(const char* name, int value);
        void setFloat(const char* name, float value);

        Shader(
            const char* vertexPath,
            const char* fragmentPath
            );
};

#endif
