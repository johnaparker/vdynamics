#include <pybind11/eigen.h>
#include <pybind11/functional.h>

#include "scene.hpp"
#include "camera.hpp"
#include "geometry/rendering_object.hpp"
#include "geometry/sphere.hpp"
#include "geometry/rectangle.hpp"

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

    py::class_<Material>(m, "Material")
        .def(py::init<float,float,float,float>(), "ambient"_a=0.3f, "diffuse"_a=1.0f, "specular"_a=0.5f, "shininess"_a=32.f);

    py::class_<RenderingObject, std::shared_ptr<RenderingObject>>(m, "RenderingObject");

    py::class_<ColoredObject, RenderingObject, std::shared_ptr<ColoredObject>>(m, "ColoredObject")
        .def_property("color", &ColoredObject::get_color, &ColoredObject::set_color);

    py::class_<Sphere, ColoredObject, std::shared_ptr<Sphere>>(m, "Sphere")
        .def(py::init<vec3, float, vec4, Material, unsigned int>(), "position"_a, "radius"_a, "color"_a, "material"_a=Material(), "resolution"_a=3)
        .def_readwrite("radius", &Sphere::radius)
        .def_property("position", &Sphere::get_position, &Sphere::set_position);

    py::class_<Rectangle, ColoredObject, std::shared_ptr<Rectangle>>(m, "Rectangle")
        .def(py::init<vec3, float, float, vec4, Material>(), "position"_a, "Lx"_a,  "Ly"_a, "color"_a, "material"_a=Material())
        .def_readwrite("Lx", &Rectangle::Lx)
        .def_readwrite("Ly", &Rectangle::Ly)
        .def_property("position", &Rectangle::get_position, &Rectangle::set_position);

    py::class_<Scene>(m, "Scene")
        .def(py::init<py::array_t<float>, py::array_t<unsigned int>>(), "background"_a, "window_size"_a)
        .def("run", &Scene::run, "callback"_a, "frames"_a)
        .def("draw", &Scene::draw, "object"_a)
        .def("add_light", &Scene::add_light, "light"_a)
        .def_readwrite("camera", &Scene::camera);

    m.def("set_install_direc", &set_install_direc, "install_direc"_a);
}
