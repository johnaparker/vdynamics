#ifndef GUARD_vec_h
#define GUARD_vec_h

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
namespace py = pybind11;

using vec2 = Eigen::Vector2f;
using vec3 = Eigen::Vector3f;
using vec4 = Eigen::Vector4f;
using Eigen::Ref;

using py_arr = py::array_t<double>;

#endif
