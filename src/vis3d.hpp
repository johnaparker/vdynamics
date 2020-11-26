#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <string>
#include <vector>
#include <functional>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "geometry/geometry.hpp"
#include <camera.hpp>

namespace py = pybind11;

void window_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void set_install_direc(std::string intall_direc);

struct Mouse {
    float xshift = 0;
    float yshift = 0;
    float window_dx = 0;
    float window_dy = 0;
    float zoom = 0;
    float lastX = 0;
    float lastY = 0;
    bool mouse_hold = false;
    bool first = true;
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
    unsigned int width;
    unsigned int height;
    float aspect_ratio;
};

struct PointLight {
    vec3 position;

    vec3& get_position() {return position;};
    void set_position(vec3 p) {position = p;};
};

class Scene {
public:
    Scene(py::array_t<float> background, py::array_t<unsigned int> window_size);

    void draw(RenderingObject& obj);
    void add_light(std::shared_ptr<PointLight> light);
    void run(std::function<void(int)> callback, int frames);

private:
    void process_input();
    void update_view(Shader& shader);

public:
    glm::vec3 background_color;
    Window window;
    Camera camera;
    Mouse mouse;

    std::vector<RenderingObject*> objects;
    std::shared_ptr<PointLight> light;

    bool paused = false;
    int current_frame = 0;
    int last_frame = 0;
};
