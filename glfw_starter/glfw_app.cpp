#include "glfw_app.h"
#include <vector>

//..................................................................................................
GlfwApp::GlfwApp():
    dragging_(false),
    zoom_buffer_(0.0),
    background_color(0.35f, 0.4f, 0.45f, 1.0f),
    tension_range_imvec(8e5, 1e5)
{}

//..................................................................................................
int32_t GlfwApp::initialize(
    GLfloat model_viewport_height,
    GLfloat x_center,
    GLfloat y_center,
    int32_t width,
    int32_t height
) {
    screen_width_ = width;
    screen_height_ = height;
    center_[0] = x_center;
    center_[1] = y_center;
    half_extents_[1] = 0.5 * model_viewport_height;
    GLfloat const screen_aspect_ratio = static_cast<GLfloat>(width) / height;
    half_extents_[0] = half_extents_[1] * screen_aspect_ratio;

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw: create a window
    window_ = glfwCreateWindow(width, height, "OpenGL Demo", NULL, NULL);
    if (window_ == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window_);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // load shaders
    if (!shader_program_.load("shader.vs", "shader.fs")) {
        return -1;
    }
    shader_program_.use();

    // set viewport uniforms
    center_uniform_ = shader_program_.get_uniform_location("center");
    half_extents_uniform_ = shader_program_.get_uniform_location("half_extents");
    shader_program_.set_uniform(center_uniform_, center_);
    shader_program_.set_uniform(half_extents_uniform_, half_extents_);

    // find tension uniforms
    tension_range_uniform = shader_program_.get_uniform_location("tension_range");

    vbo_capacity_ = 0;
    vbo_size_ = 0;

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &vbo_tensions_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_tensions_);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    // This would unbind the vertex array.
    //glBindVertexArray(0);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    const char* glsl_version = "#version 330 core";
    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    glPointSize(3.0);

    return 0;
}

//..................................................................................................
void GlfwApp::decommission() {
    // ImGui cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // GL cleanup
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    shader_program_.unload();

    // terminate, clearing all previously allocated GLFW resources
    glfwTerminate();
}

//..................................................................................................
void GlfwApp::start_frame() {
    // handle user interaction
    glfwPollEvents();
    //if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    //    glfwSetWindowShouldClose(window_, true);
    //}

    // start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

//..................................................................................................
void GlfwApp::update_vertex_buffer(float const* data, float const* tensions, uint32_t size) {
    if (size > vbo_capacity_) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_tensions_);
        glBufferData(GL_ARRAY_BUFFER, size / 2 * sizeof(float), tensions, GL_DYNAMIC_DRAW);
        vbo_capacity_ = size;
    } else {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size * sizeof(float), data);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_tensions_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size / 2 * sizeof(float), tensions);
    }
    vbo_size_ = size;
}

//..................................................................................................
void GlfwApp::update_vertex_buffer(float const* data, uint32_t size) {
    if (size > vbo_capacity_) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_tensions_);
        std::vector<float> tensions;
        tensions.resize(size / 2);
        std::fill(tensions.begin(), tensions.end(), 0.0);
        glBufferData(GL_ARRAY_BUFFER, size / 2 * sizeof(float), tensions.data(), GL_STATIC_DRAW);
        vbo_capacity_ = size;
    } else {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size * sizeof(float), data);
    }
    vbo_size_ = size;
}

//..................................................................................................
void GlfwApp::finish_frame() {
    if (dragging_ && (drag_x_ != drag_start_x_ || drag_y_ != drag_start_y_)) {
        double const dx = (drag_x_ - drag_start_x_) / screen_width_ * 2 * half_extents_[0];
        double const dy = (drag_y_ - drag_start_y_) / screen_height_ * -2 * half_extents_[1];

        center_[0] = center_start_x_ - dx;
        center_[1] = center_start_y_ - dy;
        shader_program_.set_uniform(center_uniform_, center_);
    }

    if (zoom_buffer_ != 0.0) {
        half_extents_[0] *= std::pow(0.9, zoom_buffer_);
        half_extents_[1] *= std::pow(0.9, zoom_buffer_);
        shader_program_.set_uniform(half_extents_uniform_, half_extents_);
        zoom_buffer_ = 0.0;
    }

    glClearColor(background_color.x, background_color.y, background_color.z, background_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    shader_program_.set_uniform(tension_range_uniform, tension_range_imvec);
    glDrawArrays(GL_POINTS, 0, vbo_size_ / 2);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window_);
}

//..................................................................................................
double GlfwApp::screen_to_model_coords_x(double x) {
    return (x / screen_width_ - 0.5) * 2 * half_extents_[0] + center_[0];
}

//..................................................................................................
double GlfwApp::screen_to_model_coords_y(double y) {
    return (y / screen_height_ - 0.5) * -2 * half_extents_[1] + center_[1];
}

//..................................................................................................
void GlfwApp::resize_viewport(int32_t width, int32_t height) {
    screen_width_ = width;
    screen_height_ = height;
    GLfloat const screen_aspect_ratio = static_cast<GLfloat>(width) / height;
    half_extents_[0] = half_extents_[1] * screen_aspect_ratio;
    shader_program_.set_uniform(center_uniform_, center_);
    shader_program_.set_uniform(half_extents_uniform_, half_extents_);
    glViewport(0, 0, width, height);
}

//..................................................................................................
void GlfwApp::mouse_button_callback(int button, int action, int /* mods */) {
    if (ImGui::GetIO().WantCaptureMouse) {
        return;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            center_start_x_ = center_[0];
            center_start_y_ = center_[1];
            glfwGetCursorPos(window_, &drag_start_x_, &drag_start_y_);
            drag_x_ = drag_start_x_;
            drag_y_ = drag_start_y_;
            dragging_ = true;
            //std::cout << "Clicked: " << drag_start_x_ << ", " << drag_start_y_ << '\n';
        } else if (action == GLFW_RELEASE) {
            dragging_ = false;
        }
    }
}

//..................................................................................................
void GlfwApp::cursor_position_callback(double xpos, double ypos) {
    if (!dragging_ || ImGui::GetIO().WantCaptureMouse) {
        return;
    }

    drag_x_ = xpos;
    drag_y_ = ypos;
    //std::cout << "  dragged: " << drag_x_ << ", " << drag_y_ << '\n';
}

//..................................................................................................
void GlfwApp::scroll_callback(double /* xoffset */, double yoffset) {
    if (ImGui::GetIO().WantCaptureMouse) {
        return;
    }
    zoom_buffer_ += yoffset;
    //std::cout << "scroll " << xoffset << ", " << yoffset << '\n';
}
