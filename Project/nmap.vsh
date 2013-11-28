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
smooth out vec3 v_vEyeCameraPosition;
smooth out vec3 v_vLight;
smooth out vec3 v_vEye;

void main(void)
{
    vec4 v4Vertex = vec4(a_vVertex, 1.0);
    vec4 vModelViewVertex = u_matModelView * v4Vertex;
    vec4 vObjectModelViewVertex = u_matObjectModelView * v4Vertex;
    v_vEyeCameraPosition = vec3(vObjectModelViewVertex);

    mat3 matNormal;
    matNormal[0] = u_matObjectModelView[0].xyz;
    matNormal[1] = u_matObjectModelView[1].xyz;
    matNormal[2] = u_matObjectModelView[2].xyz;
    v_vNormal = normalize(matNormal * normalize(a_vNormal));

    //mat3 matTBN = mat3(a_vTangent, a_vBitangent, a_vNormal);
    //v_vLight = matTBN * vec3(u_matModelView * vec4(0,0,0,1));
    //v_vEye = matTBN * -v_vEyeCameraPosition;

    vec3 vN = normalize(matNormal * a_vNormal);
    vec3 vT = normalize(matNormal * a_vTangent);
    vec3 vB = normalize(matNormal * a_vBitangent);

    vec3 vTmp = vec3(0.0) - v_vEyeCameraPosition;
    v_vLight.x = dot(vTmp, vT);
    v_vLight.y = dot(vTmp, vB);
    v_vLight.z = dot(vTmp, vN);

    vTmp = -v_vEyeCameraPosition;
    v_vEye.x = dot(vTmp, vT);
    v_vEye.y = dot(vTmp, vB);
    v_vEye.z = dot(vTmp, vN);

    v_vTexCoord = a_vTexCoord;
    gl_Position = u_matProjection * vObjectModelViewVertex;
}
