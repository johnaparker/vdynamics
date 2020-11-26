#pragma once

#include <string>
#include <glm/glm.hpp>

class Shader {
public:
    Shader() {};
    Shader(const std::string vshader, const std::string fshader);
    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, glm::vec3 value) const;

public:
    unsigned int ID;

private:
    void checkCompileErrors(unsigned int shader, std::string type);
};
