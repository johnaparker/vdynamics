#ifndef GUARD_CIRCLE_VIS_H
#define GUARD_CIRCLE_VIS_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <string>
#include <vector>

namespace py = pybind11;

namespace vis_2d {
    void circle_vis(py::array_t<float> pos, py::array_t<float> radii, py::array_t<float> dims, py::array_t<float> colors, py::array_t<float> background_color, py::array_t<float> edge_color, float linewidth, std::vector<std::string> texture_files, float texture_mix, const std::string vshader, const std::string fshader);
}

#endif
