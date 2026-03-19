#version 330 core

in vec3 Normal;
in vec3 Position;
out vec4 FragColor;

uniform samplerCube skybox;
uniform vec3 cameraPosition;

void main() {
    float ratio = 1.00/1.33;
    vec3 I = normalize(Position - cameraPosition);
    vec3 R = refract(I, normalize(Normal), ratio);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
