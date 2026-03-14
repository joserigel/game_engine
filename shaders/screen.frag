#version 330 core

uniform sampler2D screenTexture;

out vec4 FragColor;
in vec2 TexCoords;

void main() {
    vec3 color = texture(screenTexture, TexCoords).rgb;
    FragColor = vec4(color, 1.0);
}
