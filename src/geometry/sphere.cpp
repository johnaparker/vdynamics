#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "sphere.hpp"
#include "rigid_body.hpp"
#include <vector>

using namespace std;
using Vertices = std::vector<vec3>;
using Indices = std::vector<ivec3>;

std::pair<Vertices,Indices> sphere_vertex_data(int resolution) {
    float t = (1 + sqrt(5))/2;

    vector<vec3> vertices = {
        vec3(-1.0f, t, 0.0f),
        vec3(1.0f, t, 0.0f),
        vec3(-1.0f, -t, 0.0f),
        vec3(1.0f, -t, 0.0f),
        vec3(0.0f, -1.0f, t),
        vec3(0.0f, 1.0f, t),
        vec3(0.0f, -1.0f, -t),
        vec3(0.0f, 1.0f, -t),
        vec3(t, 0.0f, -1.0f),
        vec3(t, 0.0f, 1.0f),
        vec3(-t, 0.0f, -1.0f),
        vec3(-t, 0.0f, 1.0f),
    }; 

    vector<ivec3> indices = {
        ivec3(0, 5, 11),
        ivec3(0, 1, 5),
        ivec3(0, 7, 1),
        ivec3(0, 10, 7),
        ivec3(0, 11, 10),

        ivec3(1, 9, 5),
        ivec3(5, 4, 11),
        ivec3(11, 2, 10),
        ivec3(10, 6, 7),
        ivec3(7, 8, 1),

        ivec3(3, 4, 9),
        ivec3(3, 2, 4),
        ivec3(3, 6, 2),
        ivec3(3, 8, 6),
        ivec3(3, 9, 8),

        ivec3(4, 5, 9),
        ivec3(2, 11, 4),
        ivec3(6, 10, 2),
        ivec3(8, 7, 6),
        ivec3(9, 1, 8),
    };

    for (int n=0; n<resolution; n++) {
        vector<vec3> new_vertices;
        vector<ivec3> new_indices;
        int counter = 0;
        for (size_t i=0; i<indices.size(); i++) {
            vec3 v1 = (vertices[indices[i][0]] + vertices[indices[i][1]])/(float)(2); // 3
            vec3 v2 = (vertices[indices[i][1]] + vertices[indices[i][2]])/(float)(2); // 4
            vec3 v3 = (vertices[indices[i][2]] + vertices[indices[i][0]])/(float)(2); // 5

            new_vertices.push_back(vertices[indices[i][0]]);  // 0
            new_vertices.push_back(vertices[indices[i][1]]);  // 1
            new_vertices.push_back(vertices[indices[i][2]]); // 2
            new_vertices.push_back(v1); // 3
            new_vertices.push_back(v2); // 4
            new_vertices.push_back(v3); // 5

            new_indices.push_back(ivec3(counter+0, counter+3, counter+5));
            new_indices.push_back(ivec3(counter+4, counter+3, counter+1));
            new_indices.push_back(ivec3(counter+5, counter+4, counter+2));
            new_indices.push_back(ivec3(counter+3, counter+4, counter+5));

            counter += 6;
        }

        vertices = new_vertices;
        indices = new_indices;
    }

    for (auto& vertex: vertices) {
        vertex.normalize();
    }

    return std::make_pair(vertices, indices);
}

Sphere::Sphere(vec3 position, float radius, vec4 color, Material material, unsigned int resolution): ColoredObject(material, color), position(position), radius(radius), resolution(resolution) {
    vshader = "sphere.vs";
    fshader = "sphere.fs";
}

void Sphere::bind_vertex_data() {
    auto [vertices, indices] = sphere_vertex_data(resolution);

    bind_vertices(vertices);
    bind_indices(indices);
}

void Sphere::draw(const Shader& shader) {
    mat4 model = rigid_body_model_matrix(position, vec3::Constant(radius));

    shader.set_mat4("model", model);
    shader.set_vec4("color", color);
    material.bind_attributes(shader);

    draw_triangles(60*pow(4, resolution));
}

SphereCollection::SphereCollection(vec3_a position, Array radius, vec4_a color, Material material, unsigned int resolution): CollectionObject(material, color), position(position), radius(radius), resolution(resolution) {
    vshader = "sphere_collection.vs";
    fshader = "sphere.fs";
    Nobjects = position.rows();
    models.resize(Nobjects);
    update_models();
}

void SphereCollection::update_models() {
    for (int i = 0; i < Nobjects; i++) {
        mat4 model = rigid_body_model_matrix(position.row(i), vec3::Constant(radius(i)));
        models[i] = model;
    }
}

void SphereCollection::bind_vertex_data() {
    auto [vertices, indices] = sphere_vertex_data(resolution);

    bind_vertices(vertices);
    bind_indices(indices);

    glBindBuffer(GL_ARRAY_BUFFER, modelVBO);
    glBufferData(GL_ARRAY_BUFFER, Nobjects * sizeof(mat4), models[0].data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, Nobjects * sizeof(vec4), color.data(), GL_DYNAMIC_DRAW);
}

void SphereCollection::draw(const Shader& shader) {
    update_models();
    glBindBuffer(GL_ARRAY_BUFFER, modelVBO);
    glBufferData(GL_ARRAY_BUFFER, Nobjects * sizeof(mat4), models[0].data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, Nobjects * sizeof(vec4), color.data(), GL_DYNAMIC_DRAW);

    material.bind_attributes(shader);
    draw_triangles_instanced(60*pow(4, resolution), Nobjects);
}
