#version 330 core

in vec2 pos;
in vec4 circleColor;
out vec4 FragColor;

void main() {
    if (dot(pos, pos) > 1)
        discard;
	FragColor = circleColor;
}
