#include "vis3d.hpp"
#include "geometry/geometry.hpp"
#include "camera.hpp"
#include <pybind11/eigen.h>
#include <pybind11/functional.h>

using namespace pybind11::literals;

PYBIND11_MODULE(_vdynamics, m) {
    m.doc() = "C++/openGL module for 2D and 3D dynamic visualization";

    py::class_<Camera>(m, "Camera")
        .def(py::init<vec3,vec3,float,float>(), "position"_a, "up"_a, "yaw"_a, "pitch"_a)
        .def_property("position", &Camera::get_position, &Camera::set_position)
        .def_readwrite("yaw", &Camera::Yaw);

    py::class_<PointLight, std::shared_ptr<PointLight>>(m, "PointLight")
        .def(py::init<vec3>(), "position"_a)
        .def_property("position", &PointLight::get_position, &PointLight::set_position);

    py::class_<RenderingObject>(m, "RenderingObject")
        .def_property("color", &RenderingObject::get_color, &RenderingObject::set_color);

    py::class_<Sphere, RenderingObject>(m, "Sphere")
        .def(py::init<vec3, float, vec4>(), "position"_a, "radius"_a, "color"_a)
        .def_readwrite("radius", &Sphere::radius)
        .def_property("position", &Sphere::get_position, &Sphere::set_position);

    py::class_<Scene>(m, "Scene")
        .def(py::init<py::array_t<float>, py::array_t<unsigned int>>(), "background"_a, "window_size"_a)
        .def("run", &Scene::run, "callback"_a, "frames"_a)
        .def("draw", &Scene::draw, "object"_a)
        .def("add_light", &Scene::add_light, "light"_a)
        .def_readwrite("camera", &Scene::camera);

    m.def("set_install_direc", &set_install_direc, "install_direc"_a);
}
