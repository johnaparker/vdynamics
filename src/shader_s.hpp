#pragma once

#include <string>
#include "vec.hpp"

class Shader {
public:
    Shader() {};
    Shader(unsigned int ID): ID(ID) {};
    Shader(const std::string vshader, const std::string fshader);

    void use() const;

    void set_bool(const std::string &name, bool value) const;
    void set_int(const std::string &name, int value) const;
    void set_float(const std::string &name, float value) const;
    void set_vec3(const std::string &name, vec3& value) const;
    void set_vec4(const std::string &name, vec4& value) const;
    void set_mat3(const std::string &name, mat3& value) const;
    void set_mat4(const std::string &name, mat4& value) const;

    bool operator< (const Shader& rhs) const {return ID < rhs.ID;}
    bool operator> (const Shader& rhs) const {return ID > rhs.ID;}

public:
    unsigned int ID;

private:
    void checkCompileErrors(unsigned int shader, std::string type);
};
