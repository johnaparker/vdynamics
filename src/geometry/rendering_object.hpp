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

    void bind_vertices(std::vector<vec3>& vertices);
    void bind_indices(std::vector<ivec3>& indices);
    void draw_triangles(unsigned int N);

    virtual void bind_attribute_data();
    virtual void bind_vertex_data() = 0;
    virtual void draw(const Shader& shader) = 0;
    //virtual void draw_init(const Shader& shader);

public:
    Material material;
    std::string vshader, fshader;

protected:
    unsigned int VAO, VBO, EBO;
};

class ColoredObject: public RenderingObject {
public:
    ColoredObject(Material material, vec4 color);

    vec4& get_color() {return color;}
    void set_color(vec4 c) {color = c;}

public:
    vec4 color;
};

//class CollectionObject;

