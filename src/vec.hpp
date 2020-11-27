#pragma once

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
namespace py = pybind11;

using vec2 = Eigen::Vector2f;
using vec3 = Eigen::Vector3f;
using vec4 = Eigen::Vector4f;

using ivec2 = Eigen::Vector2i;
using ivec3 = Eigen::Vector3i;
using ivec4 = Eigen::Vector4i;

using mat2 = Eigen::Matrix<float, 2, 2>;
using mat3 = Eigen::Matrix<float, 3, 3>;
using mat4 = Eigen::Matrix<float, 4, 4>;

using Eigen::Ref;

using py_arr = py::array_t<double>;
