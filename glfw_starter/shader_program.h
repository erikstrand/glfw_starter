#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "imgui/imgui.h"
#include <string>

//--------------------------------------------------------------------------------------------------
// this is a modified version of the shader class here
// https://learnopengl.com/Getting-started/Shaders
class ShaderProgram {
public:
    ShaderProgram(): shader_id_(0) {}

    static std::string load_shader_code(char const* shader_path);
    // shader_type should be GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, or GL_FRAGMENT_SHADER
    static GLuint compile_shader(char const* shader_code, int32_t shader_type);

    // read the shader from a file and compile it (returns true on success)
    bool load(char const* vertex_path, char const* geometry_path, char const* fragment_path);
    bool load(char const* vertex_path, char const* fragment_path) {
        return load(vertex_path, nullptr, fragment_path);
    }
    void unload();

    // use/activate the shader
    void use() const { glUseProgram(shader_id_); }

    GLint get_uniform_location(char const* name) const {
        return glGetUniformLocation(shader_id_, name);
    }

    void set_uniform(GLint uniform, bool value) const { glUniform1i(uniform, (int)value); }
    void set_uniform(GLint uniform, int value) const { glUniform1i(uniform, value); }
    void set_uniform(GLint uniform, float value) const { glUniform1f(uniform, value); }
    void set_uniform(GLint uniform, ImVec2 value) const {
        glUniform2f(uniform, value.x, value.y);
    }
    void set_uniform(GLint uniform, ImVec4 value) const {
        glUniform4f(uniform, value.x, value.y, value.z, value.w);
    }
    void set_uniform(GLint uniform, glm::vec2 value) const {
        // TODO use glUniform2fv
        glUniform2f(uniform, value.x, value.y);
    }
    void set_uniform(GLint uniform, glm::mat4 const& value) const {
        glUniformMatrix4fv(uniform, 1, GL_FALSE, &value[0][0]);
    }

private:
    GLuint shader_id_;
};

#endif
