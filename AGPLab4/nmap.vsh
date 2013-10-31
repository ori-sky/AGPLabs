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

smooth out vec3 v_vVertex;
smooth out vec3 v_vNormal;
smooth out vec3 v_vTangent;
smooth out vec3 v_vBitangent;
smooth out vec2 v_vTexCoord;
smooth out vec4 v_vEyeCameraPosition;
smooth out mat3 v_matTBN;

void main(void)
{
    vec4 v4Vertex = vec4(a_vVertex, 1.0);
    vec4 vModelViewVertex = u_matModelView * v4Vertex;
    vec4 vObjectModelViewVertex = u_matObjectModelView * v4Vertex;

    mat3 matNormal;
    matNormal[0] = u_matObjectModelView[0].xyz;
    matNormal[1] = u_matObjectModelView[1].xyz;
    matNormal[2] = u_matObjectModelView[2].xyz;

    v_vVertex = a_vVertex;
    v_vNormal = normalize(matNormal * normalize(a_vNormal));
    v_vTangent = a_vTangent;
    v_vBitangent = a_vBitangent;
    v_vTexCoord = a_vTexCoord;
    v_vEyeCameraPosition = vObjectModelViewVertex;

    // tangent bitangent normal space
    v_matTBN = mat3(v_vTangent, v_vBitangent, v_vNormal);

    gl_Position = u_matProjection * vObjectModelViewVertex;
}
