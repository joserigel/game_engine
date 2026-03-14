#version 330 core

in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;

void main() {
    vec3 color = texture(diffuse_texture, TexCoord).rgb;
    FragColor = vec4(color, 1.0);
}
