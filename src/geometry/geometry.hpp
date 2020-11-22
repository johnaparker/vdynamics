#ifndef GUARD_GEOMETRY_H
#define GUARD_GEOMETRY_H

#include "shader_s.hpp"
#include "vec.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class RenderingObject {
public:
    RenderingObject(vec4 color);
    ~RenderingObject();

    void bind_attribute_data();
    void setShader(std::string vshader, std::string fshader);

    virtual void draw() = 0;
    virtual void bind_vertex_data() = 0;

    vec4 get_color() {return vec4(color.r, color.g, color.b, color.a);}
    void set_color(vec4 c) {color = glm::make_vec4(c.data());}

public:
    glm::vec4 color;
    Shader shader;

protected:
    unsigned int VAO, VBO, EBO;
    unsigned int modelVBO, colorVBO;
};

class Sphere: public RenderingObject {
public:
    Sphere(vec3 position, float radius, vec4 color);
    void draw();
    void bind_vertex_data();

public:
    vec3 position;
    float radius;
};

#endif
