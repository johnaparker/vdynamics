#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_s.hpp>
#include <iostream>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <chrono>
#include <thread>

namespace py = pybind11;
using namespace pybind11::literals;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
float aspect_ratio = (float)(SCR_WIDTH) / (float)(SCR_HEIGHT);
bool paused = false;
int current_frame = 0;
int last_frame = 0;

float xshift = 0;
float yshift = 0;
float window_dx = 0;
float window_dy = 0;
float zoom = 0;
float lastX = 0;
float lastY = 0;
bool mouse_hold = false;

int circle_vis(py::array_t<float> pos, py::array_t<float> radii, py::array_t<float> dims, py::array_t<float> colors, py::array_t<float> background_color, py::array_t<float> edge_color, float linewidth, const std::string vshader, const std::string fshader) {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader zprogram
    // ------------------------------------
    Shader shader(vshader, fshader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          
         1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    auto pos_data = pos.unchecked<3>();
    auto radii_data = radii.unchecked<1>();
    auto dims_data = dims.unchecked<2>();
    auto color_data = colors.mutable_unchecked<2>();
    auto background_color_data = background_color.unchecked<1>();
    auto edge_color_data = edge_color.unchecked<1>();

    int Ncolors = color_data.shape(0);
    int Nparticles = pos_data.shape(1);
    last_frame = pos_data.shape(0);

    glm::mat4* circleTransforms = new glm::mat4[Nparticles];;
    glm::vec4* circleColors = new glm::vec4[Nparticles];

    unsigned int transformVBO, colorVBO;
    glGenBuffers(1, &transformVBO);
    glGenBuffers(1, &colorVBO);

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, transformVBO);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);

    glEnableVertexAttribArray(5);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
    glVertexAttribDivisor(5, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        float T0 = glfwGetTime();
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(background_color_data(0), background_color_data(1), background_color_data(2), 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        float xmin = dims_data(0,0);
        float xmax = dims_data(0,1);
        float ymin = dims_data(1,0);
        float ymax = dims_data(1,1);
        float dx = xmax - xmin;
        float dy = ymax - ymin;

        if (zoom >= 0) {
            xmin = (xmin + xshift) + dx/2*(1 - 1/(1+pow(zoom,2)));
            xmax = (xmax + xshift) - dx/2*(1 - 1/(1+pow(zoom,2)));
            ymin = (ymin + yshift) + dy/2*(1 - 1/(1+pow(zoom,2)));
            ymax = (ymax + yshift) - dy/2*(1 - 1/(1+pow(zoom,2)));
        }
        else {
            xmin = (xmin + xshift) + dx/2*(zoom);
            xmax = (xmax + xshift) - dx/2*(zoom);
            ymin = (ymin + yshift) + dy/2*(zoom);
            ymax = (ymax + yshift) - dy/2*(zoom);
        }

        dx = xmax - xmin;
        dy = ymax - ymin; 
        if (dx/dy > aspect_ratio) {
            float buff = dx/aspect_ratio - dy;
            ymax = ymax + buff/2;
            ymin = ymin - buff/2;
        }
        else if (dx/dy < aspect_ratio) {
            float buff = dy*aspect_ratio - dx;
            xmax = xmax + buff/2;
            xmin = xmin - buff/2;
        }

        window_dx = xmax - xmin;
        window_dy = ymax - ymin;

        shader.use(); 

        glm::mat4 projection = glm::ortho(xmin, xmax, ymin, ymax, -0.1f, 0.1f);
        unsigned int transformLoc = glGetUniformLocation(shader.ID, "projection");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(projection));


        transformLoc = glGetUniformLocation(shader.ID, "edge_color");
        glUniform4f(transformLoc, edge_color_data(0), edge_color_data(1), edge_color_data(2), edge_color_data(3));
        glUniform1f(glGetUniformLocation(shader.ID, "linewidth"), pow(1-linewidth,2));


        for (int i = 0; i < Nparticles; i++) {
            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform, glm::vec3(pos_data(current_frame,i,0), pos_data(current_frame,i,1), 0.0f));
            transform = glm::scale(transform, glm::vec3(radii_data(i), radii_data(i), 1.0f));

            circleTransforms[i] = transform;
            int idx = i % Ncolors;
            circleColors[i] = glm::vec4(color_data(idx,0), color_data(idx,1), color_data(idx,2), color_data(idx,3));
        }
        glBindBuffer(GL_ARRAY_BUFFER, transformVBO);
        glBufferData(GL_ARRAY_BUFFER, Nparticles * sizeof(glm::mat4), &circleTransforms[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        glBufferData(GL_ARRAY_BUFFER, Nparticles * sizeof(glm::vec4), &circleColors[0], GL_DYNAMIC_DRAW);

        // render container
        glBindVertexArray(VAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, Nparticles);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        float dt = glfwGetTime() - T0;
        if (dt < 15e-3)
            std::this_thread::sleep_for(std::chrono::microseconds(15000 - (int)(dt*1e6)));
        if (!paused)
            current_frame += 1;
        if (current_frame >= pos_data.shape(0))
            current_frame = 0;
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS
     || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        current_frame += 1;
        if (current_frame >= last_frame)
            current_frame = 0;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        current_frame -= 1;
        if (current_frame < 0)
            current_frame = 0;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        current_frame = 0;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        current_frame = last_frame-1;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        xshift -= .02*window_dx;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        xshift += .02*window_dx;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        yshift -= .02*window_dy;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        yshift += .02*window_dy;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        zoom -= .1;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        zoom += .1;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        paused = !paused;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    aspect_ratio = (float)(SCR_WIDTH) / (float)(SCR_HEIGHT);
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    zoom -= 0.05*(float)yoffset;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) { 
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        if (mouse_hold) {
            float xoffset = xpos - lastX;
            float yoffset = lastY - ypos;
            xshift -= .003*xoffset*window_dx;
            yshift -= .003*yoffset*window_dy;
        }
        mouse_hold = true;
        lastX = xpos;
        lastY = ypos;
    }
    else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        mouse_hold = false;
    }
}

PYBIND11_MODULE(_vdynamics, m) {
    m.doc() = "C++/openGL module for 2D and 3D dynamic visualization";
    m.def("circle_vis", circle_vis, "position"_a,  "radii"_a, "dims"_a, "colors"_a,  "background_color"_a,  "edge_color"_a, "linewidth"_a, "vshader"_a, "fshader"_a, 
    R"pbdoc(
         Visualize dynamics of circles (2D)
    )pbdoc");
}
