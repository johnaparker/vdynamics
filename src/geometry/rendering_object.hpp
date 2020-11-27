#pragma once

#include "shader_s.hpp"
#include "vec.hpp"

class RenderingObject {
public:
    RenderingObject(vec4 color);
    ~RenderingObject();

    virtual void bind_attribute_data();
    virtual void bind_vertex_data() = 0;
    virtual void draw(const Shader& shader) = 0;

    vec4& get_color() {return color;}
    void set_color(vec4 c) {color = c;}

public:
    vec4 color;
    std::string vshader, fshader;

protected:
    unsigned int VAO, VBO, EBO;
    unsigned int modelVBO, colorVBO;
};
