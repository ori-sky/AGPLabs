#version 150
precision highp float;

uniform mat4x4 u_matProjection;
uniform mat4x4 u_matModelView;

smooth in vec4 v_vVertex;
smooth in vec3 v_vNormal;
smooth in vec4 v_vEyeCameraPosition;

out vec4 o_vColor;

void main(void)
{
    // test values
    vec3 vAmbient = vec3(0.05);
    vec3 vDiffuse = vec3(1.0, 0.4, 0.0);
    vec3 vSpecular = vec3(1.0, 0.8, 0.1);
    vec4 vLightPos = vec4(4.0, 3.0, -2.0, 1.0);

    vec3 vColor = vAmbient;

    vec3 vAux = vec3(u_matModelView * vLightPos - v_vEyeCameraPosition);
    vec3 vLightDir = normalize(vAux);
    float fDistance = length(vAux);

    // halfway vector between light direction and object
    vec3 vHalfVector = normalize(vLightDir - normalize(vec3(v_vEyeCameraPosition)));

    // normalize the interpolated normal
    vec3 vNormal = normalize(v_vNormal);

    // if light hits surface
    float fNDotL = dot(vNormal, vLightDir);
    if(fNDotL > 0.0)
    {
        float fDivisor = 1.0 +
                         fDistance * 0.0 +
                         pow(fDistance, 2.0) * 0.0 +
                         pow(fDistance, 3.0) * 0.01 +
                         pow(fDistance, 4.0) * 0.0;
        float fAttenuation = 1.0 / fDivisor;
        vColor += fAttenuation * vDiffuse * fNDotL;

        float fNDotHV = max(0.0, dot(vNormal, vHalfVector));
        vColor += vSpecular * pow(fNDotHV, 64.0);
    }

    o_vColor = vec4(vColor, 1.0);
}