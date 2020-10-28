#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in mat4 transform;
layout (location = 5) in vec4 color;

uniform mat4 projection;
out vec2 pos;
out vec4 circleColor;
out vec2 TexCoord;

void main() {
    pos = aPos.xy;
	TexCoord = vec2((aPos.x + 1)/2, (aPos.y+1)/2);
    gl_Position = projection * transform * vec4(aPos, 1.0);
    circleColor = color;
}
