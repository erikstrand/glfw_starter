#include "shader_program.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

//..................................................................................................
std::string ShaderProgram::load_shader_code(char const* shader_path) {
    std::stringstream shader_string_stream;
    try {
        std::ifstream shader_file;
        shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        shader_file.open(shader_path);
        shader_string_stream << shader_file.rdbuf();
    }
    catch (std::ifstream::failure const&) {
        std::cout << "Error: Couldn't read shader file " << shader_path << std::endl;
        throw std::runtime_error("Couldn't load shader");
    }

    // Yes, there's an unnecessary copy here. Don't think I can avoid without abanding stringstream.
    return shader_string_stream.str();
}

//..................................................................................................
GLuint ShaderProgram::compile_shader(char const* shader_code, int32_t shader_type) {
    int success;
    char info_log[512];

    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_code, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        std::cout << "Error: Shader compilation failed\n" << info_log << std::endl;
        throw std::runtime_error("Shader vertex compilation failed");
    };

    return shader;
}

//..................................................................................................
bool ShaderProgram::load(
    char const* vertex_path,
    char const* geometry_path,
    char const* fragment_path
) {
    GLuint vertex, geometry, fragment;
    try {
        std::string code;

        code = load_shader_code(vertex_path);
        vertex = compile_shader(code.c_str(), GL_VERTEX_SHADER);

        if (geometry_path != nullptr) {
            code = load_shader_code(geometry_path);
            geometry = compile_shader(code.c_str(), GL_GEOMETRY_SHADER);
        }

        code = load_shader_code(fragment_path);
        fragment = compile_shader(code.c_str(), GL_FRAGMENT_SHADER);
    }
    catch (std::runtime_error const&) {
        return false;
    }

    // link shader Program
    int success;
    char info_log[512];
    shader_id_ = glCreateProgram();
    glAttachShader(shader_id_, vertex);
    if (geometry_path != nullptr) {
        glAttachShader(shader_id_, geometry);
    }
    glAttachShader(shader_id_, fragment);
    glLinkProgram(shader_id_);
    // print linking errors if any
    glGetProgramiv(shader_id_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_id_, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
        return false;
    }

    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    if (geometry_path != nullptr) {
        glDeleteShader(geometry);
    }
    glDeleteShader(fragment);
    return true;
}

//..................................................................................................
void ShaderProgram::unload() {
    if (shader_id_ != 0) {
        glUseProgram(0);
        glDeleteProgram(shader_id_);
    }
}
