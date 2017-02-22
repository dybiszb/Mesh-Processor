#version 330 core

out vec4 color;
in vec3 fragmentNormal;
in vec3 fragmentPosition;

uniform int  u_isSelected;
uniform vec4 u_color;

void main()
{
    vec3 lightPosition =vec3(20.0, 20.0, 20.0);

    if(u_isSelected == 1) {
        color = vec4(255.0f / 256.0f, 163.0f/256.0f, 26.0f/256.0f, 1.0f);
    } else {
        vec3 N = normalize(fragmentNormal);
        vec3 lightDirection = -normalize(lightPosition);
        float NdotL = max(dot(N, lightDirection), 0.0);
        vec3 diffuse = vec3(0.5, 0.5, 0.5);
        vec3 colorInter = vec3(u_color);
        color = vec4( (diffuse * NdotL) + colorInter, 1.0);
    }
}