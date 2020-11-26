#pragma once

#include <string>

class Texture {
public:
    Texture();
    Texture(const std::string texture_path);

public:
    unsigned int ID;
};
