#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;

out vec3 fragmentNormal;
out vec3 fragmentPosition;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0f);
    fragmentNormal = vec3(a_normal);
    fragmentPosition = vec3(a_position);
}