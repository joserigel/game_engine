#version 330 core

in vec3 Normal;
in vec2 TexCoord;
in vec4 FragPosLightSpace;
in vec3 Tangent;
in vec3 Bitangent;

out vec4 FragColor;

uniform vec3 lightDir;

uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;
uniform sampler2D normals_texture;
uniform sampler2D shadowMap;

float shadowCalculation(vec4 fragPosLightSpace) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = max(0.05 * (1.0 - dot(Normal, -lightDir)), 0.005);
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    return shadow;
}

void main() {
    vec3 color = texture(diffuse_texture, TexCoord).rgb;
    vec3 normal = texture(normals_texture, TexCoord).rgb;
    normal = normal * 2.0 - 1.0;
    mat3 TBN = mat3(Tangent, Bitangent, Normal);
    normal = normalize(TBN * normal);

    float diff = max(dot(-lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    vec3 ambient = color * 0.3;

    if (shadowCalculation(FragPosLightSpace) < 0.5) {
        FragColor = vec4(diffuse + ambient, 1.0);
    } else {
        FragColor = vec4(ambient, 1.0);
    }
}
