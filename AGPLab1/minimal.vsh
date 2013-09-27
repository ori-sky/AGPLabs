#version 150
precision highp float;

in vec3 a_vVertex;
in vec3 a_vColor;
smooth out vec3 v_vColor;

void main(void)
{
    v_vColor = a_vColor;
    gl_Position = vec4(a_vVertex, 1.0);
}