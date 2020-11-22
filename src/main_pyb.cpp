#include "vis3d.hpp"
#include "geometry/geometry.hpp"
#include <pybind11/eigen.h>
#include <pybind11/functional.h>

using namespace pybind11::literals;

PYBIND11_MODULE(_vdynamics, m) {
    m.doc() = "C++/openGL module for 2D and 3D dynamic visualization";

    py::class_<RenderingObject>(m, "RenderingObject")
        .def("setShader", &RenderingObject::setShader, "vshader"_a, "fshader"_a)
        .def_property("color", &RenderingObject::get_color, &RenderingObject::set_color);

    py::class_<Sphere, RenderingObject>(m, "Sphere")
        .def(py::init<vec3, float, vec4>(), "position"_a, "radius"_a, "color"_a)
        .def_readwrite("radius", &Sphere::radius)
        .def_readwrite("position", &Sphere::position);

    py::class_<Scene>(m, "Scene")
        .def(py::init<py::array_t<float>, py::array_t<unsigned int>>(), "background"_a, "window_size"_a)
        .def("run", &Scene::run, "callback"_a, "frames"_a)
        .def("draw", &Scene::draw, "object"_a);
}
