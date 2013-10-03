#version 150
precision highp float;

smooth in vec3 v_vColor;
out vec4 o_vColor;

void main(void)
{
    o_vColor = vec4(v_vColor, 1.0);
}
