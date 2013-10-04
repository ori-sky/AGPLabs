#version 150
precision highp float;

uniform mat4x4 u_matProjection;
uniform mat4x4 u_matModelView;

in ivec3 a_vVertex;
in ivec3 a_vNormal;

smooth out vec4 v_vVertex;
smooth out vec3 v_vNormal;
smooth out vec4 v_vEyeCameraPosition;

void main(void)
{
    vec4 vVertex = vec4(a_vVertex, 1.0);
    vec4 vNormal = vec4(a_vNormal, 1.0);

    v_vVertex = vVertex;

    mat3x3 matNormal;
    matNormal[0] = u_matModelView[0].xyz;
    matNormal[1] = u_matModelView[1].xyz;
    matNormal[2] = u_matModelView[2].xyz;

    v_vNormal = normalize(matNormal * vec3(normalize(vNormal)));

    vec4 vModelViewVertex = u_matModelView * vVertex;
    v_vEyeCameraPosition = vModelViewVertex;

    gl_Position = u_matProjection * vModelViewVertex;
}
