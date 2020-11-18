#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
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
#include "sphere_vis.hpp"
#include "geometry/geometry.hpp"

namespace vis_3d {

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// global variables
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
const float deltaTime = 1.0/60.0;
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
bool firstMouse = true;
int mesh_level = 2;

GLFWwindow* window;
unsigned int VBO, VAO, EBO;
unsigned int transformVBO, colorVBO;

void make_window() {
    // initialize
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // create window
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    // set callbacks
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // enable alpha blending
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void bind_vetices() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    sphere(VBO, EBO, mesh_level);
}

void bind_attributes() {
    glGenBuffers(1, &transformVBO);
    glGenBuffers(1, &colorVBO);

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
}

void close_window() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &transformVBO);
    glDeleteBuffers(1, &colorVBO);
    glfwTerminate();
}

void update_view(unsigned int shader_ID) {
    // set uniform projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), aspect_ratio, 0.01f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader_ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glm::mat4 view = camera.GetViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(shader_ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
}

void sphere_vis(py::array_t<float> pos, py::array_t<float> radii, py::array_t<float> dims, py::array_t<float> colors, py::array_t<float> background_color, std::vector<std::string> texture_files, float texture_mix, const std::string vshader, const std::string fshader) {
    // read input arrays
    auto pos_data = pos.unchecked<3>();
    auto radii_data = radii.unchecked<1>();
    auto color_data = colors.mutable_unchecked<2>();
    auto background_color_data = background_color.unchecked<1>();

    int Ncolors = color_data.shape(0);
    int Nparticles = pos_data.shape(1);
    last_frame = pos_data.shape(0);

    // initialize OpenGL
    make_window();
    bind_vetices();
    bind_attributes();
    Shader shader(vshader, fshader);
    shader.use(); 

    // create textures
    int Ntextures = texture_files.size();
    std::vector<Texture> textures;
    for (const auto& texture_file : texture_files)
        textures.push_back(Texture(texture_file));

    // allocate instanced arrays
    glm::mat4* circleTransforms = new glm::mat4[Nparticles];;
    glm::vec4* circleColors = new glm::vec4[Nparticles];

    // set uniform data
    shader.setFloat("texture_mix", texture_mix);

    while (!glfwWindowShouldClose(window)) {
        float T0 = glfwGetTime();

        // user feedback
        processInput(window);
        update_view(shader.ID);

        shader.setVec3("lightPos", camera.Position);
        shader.setVec3("viewPos", camera.Position);

        // set background color
        glClearColor(background_color_data(0), background_color_data(1), background_color_data(2), 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        // set instanced data for all particles
        for (int i = 0; i < Nparticles; i++) {
            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform, glm::vec3(pos_data(current_frame,i,0), pos_data(current_frame,i,2), pos_data(current_frame,i,1)));
            transform = glm::scale(transform, glm::vec3(radii_data(i), radii_data(i), radii_data(i)));

            circleTransforms[i] = transform;
            int idx = i % Ncolors;
            circleColors[i] = glm::vec4(color_data(idx,0), color_data(idx,1), color_data(idx,2), color_data(idx,3));
        }

        // copy instanced data to GPU
        glBindBuffer(GL_ARRAY_BUFFER, transformVBO);
        glBufferData(GL_ARRAY_BUFFER, Nparticles * sizeof(glm::mat4), &circleTransforms[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        glBufferData(GL_ARRAY_BUFFER, Nparticles * sizeof(glm::vec4), &circleColors[0], GL_DYNAMIC_DRAW);

        // draw circles
        glBindVertexArray(VAO);
        if (Ntextures > 0)
            glBindTexture(GL_TEXTURE_2D, textures[0].ID);
        glDrawElementsInstanced(GL_TRIANGLES, 60*pow(4, mesh_level), GL_UNSIGNED_INT, 0, Nparticles);
        glfwSwapBuffers(window);
        glfwPollEvents();

        // maintrain framerate
        float dt = glfwGetTime() - T0;
        if (dt < 15e-3)
            std::this_thread::sleep_for(std::chrono::microseconds(15000 - (int)(dt*1e6)));

        // playback
        if (!paused)
            current_frame += 1;
        if (current_frame >= pos_data.shape(0))
            current_frame = 0;
    }

    close_window();
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
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


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        paused = !paused;

    if (key == GLFW_KEY_C && action == GLFW_PRESS)
        output_current_window(window, "vdynamics.png");
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    aspect_ratio = (float)(SCR_WIDTH) / (float)(SCR_HEIGHT);
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

}
