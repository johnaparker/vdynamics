#version 330 core

in vec4 circleColor;
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    vec3 ambient = vec3(0.1);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 norm = normalize(Normal);
    vec3 diffuse = vec3(max(dot(norm, lightDir), 0.0));

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = vec3(1.0 * spec);
    FragColor = vec4((ambient + diffuse + specular) * vec3(circleColor), circleColor.a);
}
