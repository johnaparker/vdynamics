#version 330 core

in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec4 color;

struct Material {
    float ambient;
    float diffuse;
    float specular;
    float shininess;
};
uniform Material material;

void main() {
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 norm = normalize(Normal);
    vec3 diffuse = material.diffuse*vec3(max(dot(norm, lightDir), 0.0));

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = vec3(material.specular * spec);

    FragColor = vec4((material.ambient + diffuse) * vec3(color.xyz + specular), color.a);
}
