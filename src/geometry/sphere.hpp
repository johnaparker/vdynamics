#pragma once

#include "rendering_object.hpp"

class Sphere: public ColoredObject {
public:
    Sphere(vec3 position, float radius, vec4 color, Material material);
    void bind_vertex_data() override;
    void draw(const Shader& shader) override;

    vec3& get_position() {return position;}
    void set_position(vec3 p) {position = p;}

public:
    vec3 position;
    float radius;
};

//class SphereCollection
    //unsigned int modelVBO, colorVBO;
