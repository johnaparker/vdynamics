#pragma once

#include "shader_s.hpp"
#include "vec.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class RenderingObject {
public:
    RenderingObject(vec4 color);
    ~RenderingObject();

    void bind_attribute_data();

    virtual void draw(const Shader& shader) = 0;
    virtual void bind_vertex_data() = 0;

    vec4& get_color() {return color;}
    void set_color(vec4 c) {color = c;}

public:
    vec4 color;
    std::string vshader, fshader;

protected:
    unsigned int VAO, VBO, EBO;
    unsigned int modelVBO, colorVBO;
};

class Sphere: public RenderingObject {
public:
    Sphere(vec3 position, float radius, vec4 color);
    void draw(const Shader& shader);
    void bind_vertex_data();

    vec3& get_position() {return position;}
    void set_position(vec3 p) {position = p;}

public:
    vec3 position;
    float radius;
};
