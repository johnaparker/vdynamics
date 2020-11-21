
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <iostream>
#include <chrono>
#include <thread>

#include <shader_s.hpp>
#include <texture_s.hpp>
#include <camera.hpp>

#include "file_io.hpp"
#include "vis3d.hpp"
#include "geometry/geometry.hpp"

void window_size_callback(GLFWwindow* window, int width, int height);
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);

Window::Window(unsigned int width, unsigned int height): width(width), height(height) {
    aspect_ratio = (float)(width) / (float)(height);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    std::cout << width << ", " << height << std::endl;
    window = glfwCreateWindow(width, height, "", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    // set callbacks
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, window_size_callback);
    //glfwSetKeyCallback(window, key_callback);
    //glfwSetScrollCallback(window, scroll_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // enable depth test and alpha blending
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Window::set_window_size(unsigned int w, unsigned int h) {
    width = w;
    height = h;
    aspect_ratio = (float)(width) / (float)(height);
}

void Window::close() {
    glfwTerminate();
}


Scene::Scene(py::array_t<float> background, py::array_t<unsigned int> window_size) {
    auto background_data = background.unchecked<1>();
    background_color = glm::vec3(background_data(0), background_data(1), background_data(2));

    auto window_size_data = window_size.unchecked<1>();
    window = Window(window_size_data(0), window_size_data(1));
}

void Scene::run() {
    while (!glfwWindowShouldClose(window.window)) {
        process_input();
        glClearColor(background_color.r, background_color.g, background_color.b, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        glfwSwapBuffers(window.window);
        glfwPollEvents();
    }
}

void Scene::process_input() {
    if (glfwGetKey(window.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window.window, true);

    //if (glfwGetKey(window.window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        //current_frame += 1;
        //if (current_frame >= last_frame)
            //current_frame = 0;
    //}

    //if (glfwGetKey(window.window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        //current_frame -= 1;
        //if (current_frame < 0)
            //current_frame = 0;
    //}

    //if (glfwGetKey(window.window, GLFW_KEY_DOWN) == GLFW_PRESS)
        //current_frame = 0;

    //if (glfwGetKey(window.window, GLFW_KEY_UP) == GLFW_PRESS)
        //current_frame = last_frame-1;

    //if (glfwGetKey(window.window, GLFW_KEY_W) == GLFW_PRESS)
        //camera.ProcessKeyboard(FORWARD, window.delta_time);
    //if (glfwGetKey(window.window, GLFW_KEY_S) == GLFW_PRESS)
        //camera.ProcessKeyboard(BACKWARD, window.delta_time);
    //if (glfwGetKey(window.window, GLFW_KEY_A) == GLFW_PRESS)
        //camera.ProcessKeyboard(LEFT, window.delta_time);
    //if (glfwGetKey(window.window, GLFW_KEY_D) == GLFW_PRESS)
        //camera.ProcessKeyboard(RIGHT, window.delta_time);
    //if (glfwGetKey(window.window, GLFW_KEY_E) == GLFW_PRESS)
        //camera.ProcessKeyboard(UP, window.delta_time);
    //if (glfwGetKey(window.window, GLFW_KEY_Q) == GLFW_PRESS)
        //camera.ProcessKeyboard(DOWN, window.delta_time);
}


//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    //if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        //paused = !paused;

    //if (key == GLFW_KEY_C && action == GLFW_PRESS)
        //output_current_window(window, "vdynamics.png");
//}

void window_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    //camera.ProcessMouseScroll(yoffset);
//}

//void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    //if (firstMouse) {
        //lastX = xpos;
        //lastY = ypos;
        //firstMouse = false;
    //}

    //float xoffset = xpos - lastX;
    //float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    //lastX = xpos;
    //lastY = ypos;

    //camera.ProcessMouseMovement(xoffset, yoffset);
//}
