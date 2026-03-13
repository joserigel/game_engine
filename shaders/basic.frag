#version 330 core

in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;

void main() {
    FragColor = texture(diffuse_texture, TexCoord);
}
