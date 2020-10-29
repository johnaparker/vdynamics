#include "file_io.hpp"
#include <stb_image/stb_image_write.h>

void output_current_window(GLFWwindow* window, std::string filename) {
    int window_w, window_h;
    glfwGetFramebufferSize(window, &window_w, &window_h);
    unsigned char* data = new unsigned char[3*window_w*window_h];
    glReadPixels(0, 0, window_w, window_h, GL_RGB, GL_UNSIGNED_BYTE, data);

    stbi_flip_vertically_on_write(true);
    stbi_write_png(filename.c_str(), window_w, window_h, 3, (void*)data, 0);
}
