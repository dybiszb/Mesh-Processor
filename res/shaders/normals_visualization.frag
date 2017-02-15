#version 330 core

out vec4 color;
varying vec3 interpolatedNormal;

uniform int  u_isSelected;
uniform vec4 u_color;

void main()
{
    if(u_isSelected == 1) {
        color = vec4(255.0f / 256.0f, 163.0f/256.0f, 26.0f/256.0f, 1.0f);
    } else {
        color = vec4(interpolatedNormal, 1.0);
    }
}