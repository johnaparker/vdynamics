#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "sphere.hpp"
#include "rigid_body.hpp"
#include <vector>

using namespace std;

Sphere::Sphere(vec3 position, float radius, vec4 color, Material material): ColoredObject(material, color), position(position), radius(radius) {
    vshader = "sphere.vs";
    fshader = "sphere.fs";
}

void Sphere::bind_vertex_data() {
    glBindVertexArray(VAO);

    float t = (1 + sqrt(5))/2;
    int precision = 2;

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
        ivec3(0, 11, 5),
        ivec3(0, 5, 1),
        ivec3(0, 1, 7),
        ivec3(0, 7, 10),
        ivec3(0, 10, 11),

        ivec3(1, 5, 9),
        ivec3(5, 11, 4),
        ivec3(11, 10, 2),
        ivec3(10, 7, 6),
        ivec3(7, 1, 8),

        ivec3(3, 9, 4),
        ivec3(3, 4, 2),
        ivec3(3, 2, 6),
        ivec3(3, 6, 8),
        ivec3(3, 8, 9),

        ivec3(4, 9, 5),
        ivec3(2, 4, 11),
        ivec3(6, 2, 10),
        ivec3(8, 6, 7),
        ivec3(9, 8, 1),
    };

    for (int i=0; i<precision; i++) {
        vector<vec3> new_vertices;
        vector<ivec3> new_indices;
        int counter = 0;
        for (int i=0; i<indices.size(); i++) {
            vec3 v1 = (vertices[indices[i][0]] + vertices[indices[i][1]])/(float)(2); // 3
            vec3 v2 = (vertices[indices[i][1]] + vertices[indices[i][2]])/(float)(2); // 4
            vec3 v3 = (vertices[indices[i][2]] + vertices[indices[i][0]])/(float)(2); // 5

            new_vertices.push_back(vertices[indices[i][0]]);  // 0
            new_vertices.push_back(vertices[indices[i][1]]);  // 1
            new_vertices.push_back(vertices[indices[i][2]]); // 2
            new_vertices.push_back(v1); // 3
            new_vertices.push_back(v2); // 4
            new_vertices.push_back(v3); // 5

            new_indices.push_back(ivec3(counter+5, counter+3, counter+0));
            new_indices.push_back(ivec3(counter+1, counter+3, counter+4));
            new_indices.push_back(ivec3(counter+2, counter+4, counter+5));
            new_indices.push_back(ivec3(counter+5, counter+4, counter+3));
            counter += 6;
        }

        vertices = new_vertices;
        indices = new_indices;
    }

    for (auto& vertex: vertices) {
        vertex.normalize();
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*3*sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*3*sizeof(int), &indices[0], GL_STATIC_DRAW);
}

void Sphere::draw(const Shader& shader) {
    mat4 model = rigid_body_model_matrix(position, vec3::Constant(radius));

    glBindVertexArray(VAO);
    shader.set_mat4("model", model);
    shader.set_vec4("color", color);
    material.bind_attributes(shader);
    glDrawElements(GL_TRIANGLES, 60*pow(4, 2), GL_UNSIGNED_INT, 0);
}
