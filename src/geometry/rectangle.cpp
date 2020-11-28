#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "rectangle.hpp"
#include "rigid_body.hpp"
#include <vector>

using namespace std;
using Vertices = std::vector<vec3>;
using Indices = std::vector<ivec3>;
using Normals = std::vector<vec3>;

std::pair<Vertices,Indices> square_vertex_data() {
    vector<vec3> vertices = {
        vec3(-0.5f, -0.5, 0.0f),
        vec3(0.0, 0.0, 1.0),
        vec3(0.5f, -0.5, 0.0f),
        vec3(0.0, 0.0, 1.0),
        vec3(0.5f, 0.5, 0.0f),
        vec3(0.0, 0.0, 1.0),
        vec3(-0.5f, 0.5, 0.0f),
        vec3(0.0, 0.0, 1.0)
    }; 

    vector<ivec3> indices = {
        ivec3(0, 1, 2),
        ivec3(0, 2, 3),
    };

    return std::make_pair(vertices, indices);
}

Rectangle::Rectangle(vec3 position, float Lx, float Ly, vec4 color, Material material): ColoredObject(material, color), position(position), Lx(Lx), Ly(Ly) {
    CullFace = false;
};

void Rectangle::bind_vertex_data() {
    auto [vertices, indices] = square_vertex_data();

    bind_vertices(vertices, 6);
    bind_indices(indices);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Rectangle::bind_attribute_data() {
    glBindVertexArray(VAO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
}

void Rectangle::draw(const Shader& shader) {
    shader.use();
    mat4 model = rigid_body_model_matrix(position, vec3(Lx,Ly,1));

    shader.set_mat4("model", model);
    shader.set_vec4("color", color);
    material.bind_attributes(shader);

    draw_triangles(6);
}
