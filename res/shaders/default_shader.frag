#version 330 core
in vec3 ourColor;

out vec4 color;
uniform int isSelected;

void main()
{
    if(isSelected == 1) {
        color = vec4(255.0f / 256.0f, 163.0f/256.0f, 26.0f/256.0f, 1.0f);
    } else {
        color = vec4(45.0f / 256.0, 137.0f / 256.0f, 239.0f / 256.0, 1.0f);
    }

}