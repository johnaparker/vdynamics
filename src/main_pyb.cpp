#include "circle_vis.hpp"
#include "sphere_vis.hpp"

using namespace pybind11::literals;

PYBIND11_MODULE(_vdynamics, m) {
    m.doc() = "C++/openGL module for 2D and 3D dynamic visualization";
    m.def("circle_vis", vis_2d::circle_vis, "position"_a,  "radii"_a, "dims"_a, "colors"_a,  "background_color"_a,  "edge_color"_a, "linewidth"_a, "texture_files"_a, "texture_mix"_a, "vshader"_a, "fshader"_a, 
    R"pbdoc(
         Visualize dynamics of circles (2D)
    )pbdoc");
    m.def("sphere_vis", vis_3d::sphere_vis, "position"_a,  "radii"_a, "dims"_a, "colors"_a,  "background_color"_a, "texture_files"_a, "texture_mix"_a, "vshader"_a, "fshader"_a, 
    R"pbdoc(
         Visualize dynamics of spheres (3D)
    )pbdoc");
}
