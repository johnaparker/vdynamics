#version 330 core

uniform vec4 circleColor;
in vec2 pos;
out vec4 FragColor;

void main() {
    if (dot(pos, pos) > 1)
        discard;
	FragColor = circleColor;
}
