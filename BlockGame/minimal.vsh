#version 150
precision highp float;

uniform mat4x4 u_matProjection;
uniform mat4x4 u_matModelView;

in ivec3 a_vVertex;
in vec3 a_vColor;
smooth out vec3 v_vColor;

void main(void)
{
    v_vColor = a_vColor;
    gl_Position = u_matProjection * u_matModelView * vec4(a_vVertex, 1);
}
