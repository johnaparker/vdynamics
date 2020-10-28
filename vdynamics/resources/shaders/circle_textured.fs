#version 330 core

in vec2 pos;
in vec4 circleColor;
in vec2 TexCoord;
out vec4 FragColor;

uniform float linewidth;
uniform vec4 edge_color;
uniform sampler2D texture1;
uniform float texture_mix;

void main() {
    float rsq = dot(pos, pos);
    if (rsq > 1)
        discard;
    if (rsq > linewidth)
        FragColor = edge_color;
    else {
        FragColor = mix(texture(texture1, TexCoord), circleColor, texture_mix);
        /*float alpha = smoothstep(0.95, 1, sqrt(rsq));*/
        /*FragColor = mix(circleColor, vec4(1,1,1,0), alpha);*/
    }
}
