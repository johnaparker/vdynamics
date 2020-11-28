#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "rendering_object.hpp"

using namespace std;

Material::Material(float ambient, float diffuse, float specular, float shininess):
    ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {};

void Material::bind_attributes(const Shader& shader) {
    shader.set_float("material.ambient", ambient);
    shader.set_float("material.diffuse", diffuse);
    shader.set_float("material.specular", specular);
    shader.set_float("material.shininess", shininess);
}

RenderingObject::RenderingObject(Material material): material(material) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

RenderingObject::~RenderingObject() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void RenderingObject::bind_vertices(std::vector<vec3>& vertices, int N) {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*N*sizeof(float), &vertices[0], GL_STATIC_DRAW);
}

void RenderingObject::bind_indices(std::vector<ivec3>& indices) {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*3*sizeof(int), &indices[0], GL_STATIC_DRAW);
}

void RenderingObject::draw_triangles(unsigned int N) {
    if (CullFace)
        glEnable(GL_CULL_FACE);  
    else
        glDisable(GL_CULL_FACE);  

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, N, GL_UNSIGNED_INT, 0);
}

void RenderingObject::bind_attribute_data() {
    glBindVertexArray(VAO);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

ColoredObject::ColoredObject(Material material, vec4 color): RenderingObject(material), color(color) {}
