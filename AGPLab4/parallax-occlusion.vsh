#version 150
precision highp float;

uniform mat4 u_matProjection;
uniform mat4 u_matModelView;
uniform mat4 u_matObjectModelView;

in vec3 a_vVertex;
in vec3 a_vNormal;
in vec3 a_vTangent;
in vec3 a_vBitangent;
in vec2 a_vTexCoord;

smooth out vec3 v_vNormal;
smooth out vec2 v_vTexCoord;

smooth out vec3 v_vTLight;
smooth out vec3 v_vTEye;

void main(void)
{
    vec4 v4Vertex = vec4(a_vVertex, 1.0);
    vec4 vModelViewVertex = u_matModelView * v4Vertex;
    vec4 vObjectModelViewVertex = u_matObjectModelView * v4Vertex;

    mat3 matNormal;
    matNormal[0] = u_matObjectModelView[0].xyz;
    matNormal[1] = u_matObjectModelView[1].xyz;
    matNormal[2] = u_matObjectModelView[2].xyz;
    v_vNormal = normalize(matNormal * normalize(a_vNormal));

    mat3 matWorldToTangent;
    matWorldToTangent[0] = matNormal * normalize(a_vTangent);
    matWorldToTangent[1] = matNormal * normalize(a_vBitangent);
    matWorldToTangent[2] = matNormal * normalize(a_vNormal);
    mat3 matTangentToWorld = transpose(matWorldToTangent);

    vec3 vEye = vec3(vObjectModelViewVertex);

    v_vTLight = (vec3(0.0) - vEye) * matWorldToTangent;
    v_vTEye = -vEye * matWorldToTangent;

    v_vTexCoord = a_vTexCoord;
    gl_Position = u_matProjection * vObjectModelViewVertex;
}
