#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 transform;
uniform mat4 projection;
out vec2 pos;

void main() {
    pos = aPos.xy;
	gl_Position = projection * transform * vec4(aPos, 1.0);
}
