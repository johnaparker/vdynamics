#version 330 core

in vec2 pos;
in vec4 circleColor;
out vec4 FragColor;

uniform float linewidth;
uniform vec4 edge_color;

void main() {
    float rsq = dot(pos, pos);
    if (rsq > 1)
        discard;
    if (rsq > linewidth)
        FragColor = edge_color;
    else {
        FragColor = circleColor;
        /*float alpha = smoothstep(0.95, 1, sqrt(rsq));*/
        /*FragColor = mix(circleColor, vec4(1,1,1,0), alpha);*/
    }
}
