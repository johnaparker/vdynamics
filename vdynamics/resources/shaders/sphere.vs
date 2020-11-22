#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform vec4 color;
out vec4 circleColor;
out vec3 Normal;
out vec3 FragPos;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    circleColor = color;
    Normal = aPos;
    FragPos = vec3(model * vec4(aPos,1.0));
}
