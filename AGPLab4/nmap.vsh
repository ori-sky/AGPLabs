#version 150
precision highp float;

uniform mat4 u_matProjection;
uniform mat4 u_matModelView;
uniform mat4 u_matObjectModelView;

in ivec3 a_vVertex;
in ivec3 a_vNormal;
in vec2 a_vTexCoord;
smooth out vec4 v_vVertex;
smooth out vec3 v_vNormal;
smooth out vec2 v_vTexCoord;

smooth out vec4 v_vEyeCameraPosition;
smooth out mat3 v_matNormal;

void main(void)
{
    vec4 vVertex = vec4(a_vVertex, 1.0);
    vec4 vModelViewVertex = u_matModelView * vVertex;
    vec4 vObjectModelViewVertex = u_matObjectModelView * vVertex;

    v_vVertex = vVertex;
    v_vEyeCameraPosition = vObjectModelViewVertex;

    mat3 matNormal;
    matNormal[0] = u_matObjectModelView[0].xyz;
    matNormal[1] = u_matObjectModelView[1].xyz;
    matNormal[2] = u_matObjectModelView[2].xyz;
    v_matNormal = matNormal;

    vec3 vNormal = vec3(a_vNormal);
    v_vNormal = normalize(matNormal * normalize(vNormal));

    v_vTexCoord = a_vTexCoord;

    gl_Position = u_matProjection * vObjectModelViewVertex;
}
