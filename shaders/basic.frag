#version 330 core

in vec3 Normal;
in vec2 TexCoord;
in vec4 FragPosLightSpace;
in vec3 FragPos;
in vec3 Tangent;
in vec3 Bitangent;

out vec4 FragColor;

uniform vec3 lightDir;
uniform vec3 cameraPosition;

uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;
uniform sampler2D normals_texture;
uniform sampler2D height_texture;
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

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir) {
    float height = texture(height_texture, texCoords).r;
    const float numLayers = 10;
    float layerDepth = 1.0 / numLayers;
    float currentDepth = 0.0;
    vec2 p = viewDir.xy * 0.5;
    vec2 deltaTexCoords = p / numLayers;
    vec2 currentTexCoords = texCoords;
    float currentDepthValue = texture(height_texture, currentTexCoords).r;

    while (currentDepth < currentDepthValue) {
        currentTexCoords -= deltaTexCoords;
        currentDepthValue = texture(height_texture, currentTexCoords).r;
        currentDepth += layerDepth;
    }
    
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    float afterDepth  = currentDepthValue - currentDepth;
    float beforeDepth = texture(height_texture, prevTexCoords).r - currentDepth + layerDepth;
 
    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;  
}

void main() {
    mat3 TBN = mat3(Tangent, Bitangent, Normal);
    vec3 viewDir = TBN * (cameraPosition - FragPos);
    vec2 texCoords = ParallaxMapping(TexCoord, viewDir);

    if(
        texCoords.x > 1.0 ||
        texCoords.y > 1.0 ||
        texCoords.x < 0.0 ||
        texCoords.y < 0.0
        ) {
        discard;
    }

    vec3 color = texture(diffuse_texture, texCoords).rgb;
    vec3 normal = texture(normals_texture, texCoords).rgb;
    normal = normal * 2.0 - 1.0;
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
