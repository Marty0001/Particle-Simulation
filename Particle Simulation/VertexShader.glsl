#version 330 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec3 aColor;

out vec3 color;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(aPosition, 0.0, 1.0);
    color = aColor;
}
