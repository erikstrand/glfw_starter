#include "glfw_app.h"
#include <vector>

//--------------------------------------------------------------------------------------------------
GlfwApp app;

// These methods let us feed GLFW regular function pointers instead of pointers to member functions.
void resize_viewport(GLFWwindow*, int width, int height) {
    app.resize_viewport(width, height);
}
void mouse_button_callback(GLFWwindow*, int button, int action, int mods) {
    app.mouse_button_callback(button, action, mods);
}
void cursor_position_callback(GLFWwindow*, double xpos, double ypos) {
    app.cursor_position_callback(xpos, ypos);
}
void scroll_callback(GLFWwindow*, double xoffset, double yoffset) {
    app.scroll_callback(xoffset, yoffset);
}

//--------------------------------------------------------------------------------------------------
int main() {
    // Configuration
    float const x_min = -1.1;
    float const y_min = -1.1;
    float const x_max = 1.1;
    float const y_max = 1.1;
    uint32_t const initial_width = 800;
    uint32_t const initial_height = 600;

    // Initialize the GUI
    int32_t const app_initialized = app.initialize(
        y_max - y_min,         // viewport height
        0.5 * (x_min + x_max), // x center
        0.5 * (y_min + y_max), // y center
        initial_width,
        initial_height
    );
    if (app_initialized != 0) {
        return -1;
    }
    app.set_framebuffer_size_callback(resize_viewport);
    app.set_mouse_button_callback(mouse_button_callback);
    app.set_cursor_position_callback(cursor_position_callback);
    app.set_scroll_callback(scroll_callback);

    // Packed x_0, y_0, x_1, y_1, etc.
    std::vector<float> particle_positions;
    // This attaches one scalar value to each particle, used to set its color in the vertex shader.
    // It's called tension since when I implemented it I was measuring internal stresses in a
    // physical simulation. Packed t_0, t_1, etc.
    std::vector<float> particle_tensions;
    uint32_t const n_particles = 1000;
    particle_positions.resize(2 * n_particles);
    particle_tensions.resize(n_particles);

    // Let's make the particles move along a Lissajous curve.
    float alpha = 5.0;
    float beta = 4.0;
    float delta = 3.1415926 / 4;
    auto const update_points = [&](float t) {
        for (uint32_t i = 0; i < n_particles; ++i) {
            float const t_offset = 2 * 3.1415926 * i / n_particles;
            float const t_prime = t + t_offset;
            float const x = std::sin(alpha * t_prime + delta);
            float const y = std::sin(beta * t_prime);
            particle_positions[2 * i] = x;
            particle_positions[2 * i + 1] = y;
            particle_tensions[i] = static_cast<float>(i) / n_particles;
        }
    };

    float time = 0.0;
    float delta_t = 1e-3;
    while (app.active()) {
        app.start_frame();

        update_points(time);
        time += delta_t;

        app.update_vertex_buffer(
            particle_positions.data(),
            particle_tensions.data(),
            particle_positions.size()
        );

        ImGui::Begin("OpenGL Demo");
        auto const framerate = ImGui::GetIO().Framerate;
        ImGui::Text("%.1f ms/frame (%.1f FPS)", 1000.0f / framerate, framerate);
        ImGui::Text("points: %u", static_cast<uint32_t>(particle_positions.size()) / 2);
        ImGui::Text("t: %.3f", time);
        ImGui::SliderFloat("delta t", &delta_t, 0.0f, 0.01f, "%.3e");
        ImGui::SliderFloat("alpha", &alpha, 0.0f, 10.0f, "%.2f");
        ImGui::SliderFloat("beta", &beta, 0.0f, 10.0f, "%.2f");
        ImGui::SliderFloat("delta", &delta, 0.0f, 10.0f, "%.2f");
        ImGui::ColorEdit3("background_color", reinterpret_cast<float*>(&app.background_color));
        ImGui::InputFloat("tension scale min", &app.tension_range_imvec.x, 0.0f, 1.0f, "%.2f");
        ImGui::InputFloat("tension scale max", &app.tension_range_imvec.y, 0.0f, 1.0f, "%.2f");
        ImGui::SliderFloat("point size", &app.point_size, 1.0f, 10.0f, "%.2f");
        ImGui::End();

        app.finish_frame();
    }

    app.decommission();
    return 0;
}
