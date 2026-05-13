#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;


out vec3 Normal;
out vec2 TexCoord;
out vec4 FragPosLightSpace;
out vec3 Tangent;
out vec3 Bitangent;

uniform mat4 projection;
uniform mat4 lightMatrix;

void main() {
    Normal = aNormal;
    Tangent = normalize(aTangent - dot(aTangent, aNormal) * aNormal);
    Bitangent = cross(aNormal, aTangent);
    gl_Position = projection * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    FragPosLightSpace = lightMatrix * vec4(aPos, 1.0);
}
