#ifndef GUARD_VIS_3D_H
#define GUARD_VIS_3D_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace py = pybind11;

class Mouse {
    float xshift = 0;
    float yshift = 0;
    float window_dx = 0;
    float window_dy = 0;
    float zoom = 0;
    float lastX = 0;
    float lastY = 0;
    bool mouse_hold = false;
    bool firstMouse = true;
};


class Window {
public:
    Window(unsigned int w, unsigned int h);
    Window() {};
    void set_window_size(unsigned int w, unsigned int h);
    void close();

public:
    GLFWwindow* window;
    float delta_time = 1.0/60.0;

private:
    unsigned int width;
    unsigned int height;
    float aspect_ratio;
};

class Scene {
public:
    Scene(py::array_t<float> background, py::array_t<unsigned int> window_size);

    void run();
    //void draw(Object obj);
    //void run(std::function<> callback);

private:
    void process_input();
    //void update_view();

private:
    glm::vec3 background_color;
    Window window;
    //Mouse mouse;
    //Camera camera;

    //std::vector<Object> objects;
    //bool paused;
    //int current_frame;
    //int last_frame;
};

#endif
