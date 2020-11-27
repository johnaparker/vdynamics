#include "rigid_body.hpp"

mat4 rigid_body_model_matrix(const vec3& position, const vec3& scale) {
    mat4 model = mat4::Identity();
    model.diagonal() << scale;
    model.block<3,1>(0,3) = position;

    return model;
}
