#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 OriginalNormal;

out vec4 FragColor;

uniform sampler2D diffuse_texture;

void main() {
    vec3 lightDir = normalize(vec3(0.5, 1.0, 1.0));
    vec3 color = texture(diffuse_texture, TexCoord).rgb;

    vec3 final = max(0, dot(lightDir, Normal)) * color * 0.9;

    if (OriginalNormal.x == 1.0) {
        final += vec3(0.1, 0.0, 0.0);
    } else if (OriginalNormal.y == 1.0) {
        final += vec3(0.0, 0.1, 0.0);
    } else if (OriginalNormal.z == 1.0) {
        final += vec3(0.0, 0.0, 0.1);
    } else {
        final += color * 0.1;
    }
    FragColor = vec4(final, 1.0);
}
