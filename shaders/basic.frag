#version 330 core

in vec3 Normal;

out vec4 FragColor;

void main() {
    vec3 light = normalize(vec3(-0.5, -0.5, -0.5));
    float shaded = normalize(dot(light, Normal));
    FragColor = vec4(Normal, 1.0);
}
