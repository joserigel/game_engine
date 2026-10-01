#version 330 core

uniform sampler2D bitmapFont;
uniform vec2 position;

out vec4 FragColor;
in vec2 TexCoords;

void main() {
    vec2 coords = Texcords + position;
    vec3 color = texture(bitmapFont, coords).rgb;
    if (color.x == 0) {
        discard;
    }
    FragColor = vec4(color, 1.0);
}
