#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 projection;

out vec3 Normal;
out vec3 Position;

void main() {
    Normal = mat3(transpose(inverse(projection))) * aNormal;
    Position = aPos;
    gl_Position = projection * vec4(aPos, 1.0);
}
