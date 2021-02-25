#version 330 core

layout (location = 0) in vec2 x;
layout (location = 1) in float tension;

uniform vec2 center;
uniform vec2 half_extents;
uniform vec2 tension_range;

out vec3 fragment_color;

void main() {
    gl_Position = vec4(
        (x[0] - center[0]) / half_extents[0],
        (x[1] - center[1]) / half_extents[1],
        0.0,
        1.0
    );

    float scaled_tension = (tension - tension_range[0]) / (tension_range[1] - tension_range[0]);
    fragment_color = vec3(
        clamp(scaled_tension, 0.0, 1.0),
        0.0,
        0.2
    );
}
