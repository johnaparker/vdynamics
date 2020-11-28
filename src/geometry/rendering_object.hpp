#pragma once

#include "shader_s.hpp"
#include "vec.hpp"
#include <vector>

class Material {
public:
    Material(float ambient=0.3f, float diffuse=1.0f, float specular=0.5f, float shininess=32.f);
    void bind_attributes(const Shader& shader);

public:
    float ambient;
    float diffuse;
    float specular;
    float shininess;
};

class RenderingObject {
public:
    RenderingObject(Material material);
    ~RenderingObject();

    void bind_vertices(std::vector<vec3>& vertices, int N=3);
    void bind_indices(std::vector<ivec3>& indices);
    void draw_triangles(unsigned int N);

    virtual void bind_attribute_data();
    virtual void bind_vertex_data() = 0;
    virtual void draw(const Shader& shader) = 0;
    //virtual void draw_init(const Shader& shader);

public:
    Material material;
    std::string vshader = "default.vs";
    std::string fshader = "default.fs";

protected:
    unsigned int VAO, VBO, EBO;
    bool CullFace = true;
};

class ColoredObject: public RenderingObject {
public:
    ColoredObject(Material material, vec4 color);

    vec4& get_color() {return color;}
    void set_color(vec4 c) {color = c;}

public:
    vec4 color;
};

class CollectionObject: public RenderingObject {
public:
    CollectionObject(Material material, vec4_a color);
    void draw_triangles_instanced(unsigned int N, unsigned int M);

    virtual void bind_attribute_data() override;

    vec4_a& get_color() {return color;}
    void set_color(const vec4_a& c) {color = c;}

public:
    unsigned int modelVBO, colorVBO;
    vec4_a color;
};
