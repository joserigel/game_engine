#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D diffuse_texture;

void main() {
    vec3 color = texture(diffuse_texture, TexCoord).rgb;
    FragColor = vec4(color, 1.0);
}
