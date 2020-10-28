#ifndef GUARD_texture_s_h
#define GUARD_texture_s_h

#include <string>

class Texture {
public:
    Texture();
    Texture(const std::string texture_path);

public:
    unsigned int ID;
};

#endif
