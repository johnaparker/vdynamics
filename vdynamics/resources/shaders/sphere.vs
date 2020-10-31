#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in mat4 transform;
layout (location = 5) in vec4 color;

uniform mat4 view;
uniform mat4 projection;
out vec4 circleColor;

void main() {
    gl_Position = projection * view * transform * vec4(aPos, 1.0);
    circleColor = color;
}
