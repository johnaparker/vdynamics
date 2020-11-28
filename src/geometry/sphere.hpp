#pragma once

#include "rendering_object.hpp"

class Sphere: public ColoredObject {
public:
    Sphere(vec3 position, float radius, vec4 color, Material material, unsigned int resolution=3);
    void bind_vertex_data() override;
    void draw(const Shader& shader) override;

    vec3& get_position() {return position;}
    void set_position(const vec3& p) {position = p;}

public:
    vec3 position;
    float radius;

private:
    unsigned int resolution;
};

class SphereCollection: public CollectionObject {
public:
    SphereCollection(vec3_a position, Array radius, vec4_a color, Material material, unsigned int resolution=2);
    void bind_vertex_data() override;
    void draw(const Shader& shader) override;
    void update_models();

    vec3_a& get_position() {return position;}
    void set_position(const vec3_a& p) {position = p;}

    Array& get_radius() {return radius;}
    void set_radius(const Array& r) {radius = r;}

public:
    vec3_a position;
    Array radius;

private:
    std::vector<mat4> models;
    unsigned int resolution;
    unsigned int Nobjects;
};
