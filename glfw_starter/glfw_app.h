#ifndef GLFW_APP_H
#define GLFW_APP_H

#include <cmath>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "shader_program.h"

//--------------------------------------------------------------------------------------------------
class GlfwApp {
public:
    GlfwApp();

    // returns 0 on success
    int32_t initialize(
        GLfloat model_viewport_height,
        GLfloat x_center,
        GLfloat y_center,
        int32_t width,
        int32_t height
    );
    void decommission();

    bool active() const { return !glfwWindowShouldClose(window_); }
    void start_frame();
    ShaderProgram const& shader_program() const { return shader_program_; }
    void update_vertex_buffer(float const* data, float const* tensions, uint32_t size);
    void update_vertex_buffer(float const* data, uint32_t size);
    void finish_frame();

    double screen_to_model_coords_x(double x);
    double screen_to_model_coords_y(double y);

    void resize_viewport(int32_t width, int32_t height);
    void set_framebuffer_size_callback(GLFWframebuffersizefun callback) {
        glfwSetFramebufferSizeCallback(window_, callback);
    }

    void mouse_button_callback(int button, int action, int mods);
    void set_mouse_button_callback(GLFWmousebuttonfun callback) {
        glfwSetMouseButtonCallback(window_, callback);
    }

    void cursor_position_callback(double xpos, double ypos);
    void set_cursor_position_callback(GLFWcursorposfun callback) {
        glfwSetCursorPosCallback(window_, callback);
    }

    void scroll_callback(double xoffset, double yoffset);
    void set_scroll_callback(GLFWscrollfun callback) {
        glfwSetScrollCallback(window_, callback);
    }

private:
    GLFWwindow* window_;
    ShaderProgram shader_program_;
    uint32_t vbo_capacity_;
    uint32_t vbo_size_;
    GLuint vbo_;
    GLuint vbo_tensions_;
    GLuint vao_;

    double screen_width_;
    double screen_height_;
    // this point (in world coords) gets mapped to the center of the screen
    glm::vec2 center_;
    // defines the half width and half height of the visible area (in world coords)
    glm::vec2 half_extents_;
    GLint center_uniform_;
    GLint half_extents_uniform_;

    double center_start_x_;
    double center_start_y_;
    double drag_start_x_;
    double drag_start_y_;
    double drag_x_;
    double drag_y_;
    bool dragging_;
    double zoom_buffer_;

public:
    ImVec4 background_color;
    GLint tension_range_uniform;
    ImVec2 tension_range_imvec;
};

#endif
