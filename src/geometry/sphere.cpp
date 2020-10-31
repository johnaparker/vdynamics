#include <vector>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "geometry.hpp"

using namespace std;

void sphere(const int& VBO, const int& EBO, int precision) {
    float t = (1 + sqrt(5))/2;

    vector<glm::vec3> vertices = {
        glm::vec3(-1.0f, t, 0.0f),
        glm::vec3(1.0f, t, 0.0f),
        glm::vec3(-1.0f, -t, 0.0f),
        glm::vec3(1.0f, -t, 0.0f),
        glm::vec3(0.0f, -1.0f, t),
        glm::vec3(0.0f, 1.0f, t),
        glm::vec3(0.0f, -1.0f, -t),
        glm::vec3(0.0f, 1.0f, -t),
        glm::vec3(t, 0.0f, -1.0f),
        glm::vec3(t, 0.0f, 1.0f),
        glm::vec3(-t, 0.0f, -1.0f),
        glm::vec3(-t, 0.0f, 1.0f),
    }; 
    vector<glm::ivec3> indices = {
        glm::ivec3(0, 11, 5),
        glm::ivec3(0, 5, 1),
        glm::ivec3(0, 1, 7),
        glm::ivec3(0, 7, 10),
        glm::ivec3(0, 10, 11),

        glm::ivec3(1, 5, 9),
        glm::ivec3(5, 11, 4),
        glm::ivec3(11, 10, 2),
        glm::ivec3(10, 7, 6),
        glm::ivec3(7, 1, 8),

        glm::ivec3(3, 9, 4),
        glm::ivec3(3, 4, 2),
        glm::ivec3(3, 2, 6),
        glm::ivec3(3, 6, 8),
        glm::ivec3(3, 8, 9),

        glm::ivec3(4, 9, 5),
        glm::ivec3(2, 4, 11),
        glm::ivec3(6, 2, 10),
        glm::ivec3(8, 6, 7),
        glm::ivec3(9, 8, 1),
    };

    for (int i=0; i<precision; i++) {
        vector<glm::vec3> new_vertices;
        vector<glm::ivec3> new_indices;
        int counter = 0;
        for (int i=0; i<indices.size(); i++) {
            glm::vec3 v1 = (vertices[indices[i][0]] + vertices[indices[i][1]])/(float)(2); // 3
            glm::vec3 v2 = (vertices[indices[i][1]] + vertices[indices[i][2]])/(float)(2); // 4
            glm::vec3 v3 = (vertices[indices[i][2]] + vertices[indices[i][0]])/(float)(2); // 5

            new_vertices.push_back(vertices[indices[i][0]]);  // 0
            new_vertices.push_back(vertices[indices[i][1]]);  // 1
            new_vertices.push_back(vertices[indices[i][2]]); // 2
            new_vertices.push_back(v1); // 3
            new_vertices.push_back(v2); // 4
            new_vertices.push_back(v3); // 5

            new_indices.push_back(glm::ivec3(counter+0, counter+3, counter+5));
            new_indices.push_back(glm::ivec3(counter+1, counter+3, counter+4));
            new_indices.push_back(glm::ivec3(counter+2, counter+4, counter+5));
            new_indices.push_back(glm::ivec3(counter+3, counter+4, counter+5));
            counter += 6;
        }

        vertices = new_vertices;
        indices = new_indices;
    }

    for (auto& vertex: vertices) {
        vertex = glm::normalize(vertex);
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*3*sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*3*sizeof(int), &indices[0], GL_STATIC_DRAW);
}
