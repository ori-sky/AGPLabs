#version 150
precision highp float;

uniform sampler2D u_texture;
uniform sampler2D u_nmap;
uniform sampler2D u_hmap;

uniform mat4 u_matProjection;
uniform mat4 u_matModelView;
uniform mat4 u_matObjectModelView;

smooth in vec3 v_vNormal;
smooth in vec2 v_vTexCoord;
smooth in vec3 v_vEyeCameraPosition;
smooth in vec3 v_vLight;
smooth in vec3 v_vEye;

out vec4 o_vColor;

/*void main(void)
{
    vec2 vTexCoord = v_vTexCoord.st;
    float height = texture(u_nmap, vTexCoord).r;
    float fHSB = height * 0.04 + 0.02;
    vTexCoord += normalize(v_vEyeVec).xy * fHSB;

    o_vColor = texture(u_texture, vTexCoord);
}*/

void main(void)
{
    vec3 vNormal = texture(u_nmap, v_vTexCoord.st).rgb * 2.0 - 1.0;

    float fDistance = length(v_vLight);
    vec3 vLight = normalize(v_vLight);

    // attenuation
    float fDivisor = 1.0 +
                     fDistance * 0.05 +
                     pow(fDistance, 2.0) * 0.01 +
                     pow(fDistance, 3.0) * 0.002 +
                     pow(fDistance, 4.0) * 0.002;
    float fAttenuation = 1.0 / fDivisor;

    // ambient
    vec3 vAmbient = vec3(0.05);

    // diffuse
    float fNDotL = max(0.0, dot(vNormal, vLight));
    vec3 vDiffuse = vec3(0.0, 0.9, 1.0) * fNDotL * fAttenuation;

    // specular
    vec3 vSpecular = vec3(0.0);
    if(fNDotL > 0.0)
    {
        vec3 vHalf = normalize(vLight + normalize(v_vEye));
        float fNDotH = max(0.0, dot(vNormal, vHalf));
        vSpecular = vec3(0.5, 1.0, 1.0) * pow(fNDotH, 64.0) * fAttenuation;
    }

    // texture
    vec3 vTexColor = texture(u_texture, v_vTexCoord.st).rgb;

    // output
    o_vColor = vec4(vAmbient + vDiffuse * vTexColor + vSpecular, 1.0);
}
