#pragma once

#include "rendering_object.hpp"

class Rectangle: public ColoredObject {
public:
    Rectangle(vec3 position, float Lx, float Ly, vec4 color, Material material);
    void bind_vertex_data() override;
    void draw(const Shader& shader) override;
    void bind_attribute_data() override;

    vec3& get_position() {return position;}
    void set_position(vec3 p) {position = p;}

public:
    vec3 position;
    float Lx, Ly;
};
