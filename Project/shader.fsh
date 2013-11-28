#version 150
precision highp float;

uniform sampler2D u_sDiffuse;
uniform sampler2D u_sNormalHeight;
uniform sampler2D u_sSpecular;

uniform mat4 u_matProjection;
uniform mat4 u_matModelView;
uniform mat4 u_matObjectModelView;

smooth in vec3 v_vNormal;
smooth in vec2 v_vTexCoord;
smooth in vec3 v_vEye;

smooth in vec3 v_vTLight;
smooth in vec3 v_vTEye;
smooth in vec3 v_vTNormal;

out vec4 o_vColor;

const float fParallaxScale = 0.15;
const float fParallaxMaxSamples = 250;
const float fParallaxMinSamples = 20;

vec2 parallax_occlusion_mapping(sampler2D sMap, vec2 vTexCoord, vec3 vEye, vec3 vNormal, float fScale, float fMaxSamples, float fMinSamples)
{
    float fParallaxLimit = -length(vEye.xy) / vEye.z * fScale;

    vec2 vOffsetDir = normalize(vEye.xy);
    vec2 vMaxOffset = vOffsetDir * fParallaxLimit;

    float fEDotN = dot(normalize(vEye), vNormal);
    int nNumSamples = int(mix(fMaxSamples, fMinSamples, fEDotN));
    float fStepSize = 1.0 / float(nNumSamples);

    vec2 dx = dFdx(vTexCoord);
    vec2 dy = dFdy(vTexCoord);

    vec2 vCurrOffset = vec2(0.0);
    vec2 vLastOffset = vec2(0.0);
    float fCurrRayHeight = 1.0;
    float fLastSampledHeight = 1.0;
    float fCurrSampledHeight = 1.0;

    for(int nCurrSample=0; nCurrSample<nNumSamples;)
    {
        fCurrSampledHeight = textureGrad(sMap, vTexCoord + vCurrOffset, dx, dy).a;
        if(fCurrSampledHeight > fCurrRayHeight)
        {
            float delta1 = fCurrSampledHeight - fCurrRayHeight;
            float delta2 = (fCurrRayHeight + fStepSize) - fLastSampledHeight;

            float ratio = delta1 / (delta1 + delta2);
            vCurrOffset = (ratio) * vLastOffset + (1.0 - ratio) * vCurrOffset;
            nCurrSample = nNumSamples + 1;
        }
        else
        {
            ++nCurrSample;
            fCurrRayHeight -= fStepSize;
            vLastOffset = vCurrOffset;
            vCurrOffset += fStepSize * vMaxOffset;
            fLastSampledHeight = fCurrSampledHeight;
        }
    }

    return vTexCoord + vCurrOffset;
}

vec3 normal_mapping(sampler2D sMap, vec2 vTexCoord)
{
    vec3 vNormal = texture(sMap, vTexCoord).rgb * 2.0 - 1.0;

    // convert normal from texture coords to OpenGL coords
    // texture coords are top-left based
    // OpenGL coords are bottom-left based
    vNormal.y *= -1;

    return vNormal;
}

void main(void)
{
    // parallax occlusion mapping
    vec2 vTexCoord = parallax_occlusion_mapping(u_sNormalHeight, v_vTexCoord, v_vTEye, v_vTNormal, 0.15, 128, 16);

    // normal mapping
    vec3 vNormal = normal_mapping(u_sNormalHeight, vTexCoord);

    float fDistance = length(v_vTLight);
    vec3 vLight = normalize(v_vTLight);

    // attenuation
    float fDivisor = 1.0 +     fDistance * 0.05 +
                     pow(fDistance, 2.0) * 0.03 +
                     pow(fDistance, 3.0) * 0.01 +
                     pow(fDistance, 4.0) * 0.00;
    float fAttenuation = 1.0 / fDivisor;

    // ambient
    vec3 vAmbient = vec3(0.05);

    // diffuse
    float fNDotL = max(0.0, dot(vNormal, vLight));
    vec3 vDiffuse = vec3(0.8, 0.75, 0.7) * fNDotL * fAttenuation;

    // specular
    vec3 vSpecular = vec3(0.0);
    if(fNDotL > 0.0)
    {
        vec3 vHalf = normalize(vLight + normalize(v_vTEye));
        float fNDotH = max(0.0, dot(vNormal, vHalf));
        vSpecular = vec3(0.8, 0.8, 0.8) * pow(fNDotH, 16.0) * fAttenuation;
        vSpecular *= texture(u_sSpecular, vTexCoord).rgb;
    }

    // texture
    vec3 vTexColor = texture(u_sDiffuse, vTexCoord).rgb;

    // output
    o_vColor = vec4(vAmbient + vDiffuse * vTexColor + vSpecular, 1.0);
}
