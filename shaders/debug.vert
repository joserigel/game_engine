#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 Normal;
out vec3 OriginalNormal;

uniform mat4 projection;
uniform mat4 local;

void main() {
    TexCoord = aTexCoord;
    OriginalNormal = aNormal;
    Normal = normalize(transpose(inverse(mat3(local))) * aNormal);
    gl_Position = projection * local * vec4(aPos, 1.0);
}
