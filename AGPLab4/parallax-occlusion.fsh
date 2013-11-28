#version 150
precision highp float;

uniform sampler2D u_texture;
uniform sampler2D u_nmap;
uniform sampler2D u_glossmap;

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

void main(void)
{
    // initial texcoord
    vec2 vTexCoord = v_vTexCoord.st;

    // parallax occlusion mapping

    vec3 vL = normalize(v_vTLight);
    vec3 vE = normalize(v_vTEye);
    vec3 vN = normalize(v_vTNormal);

    float fParallaxLimit = -length(v_vTEye.xy) / v_vTEye.z;
    fParallaxLimit *= fParallaxScale; // scale

    vec2 vOffsetDir = normalize(v_vTEye.xy);
    vec2 vMaxOffset = vOffsetDir * fParallaxLimit;

    int nNumSamples = int(mix(fParallaxMaxSamples, fParallaxMinSamples, dot(vE, vN)));
    float fStepSize = 1.0 / float(nNumSamples);

    vec2 dx = dFdx(v_vTexCoord.st);
    vec2 dy = dFdy(v_vTexCoord.st);

    float fCurrRayHeight = 1.0;
    vec2 vCurrOffset = vec2(0.0);
    vec2 vLastOffset = vec2(0.0);
    float fLastSampledHeight = 1.0;
    float fCurrSampledHeight = 1.0;
    int nCurrSample = 0;

    // for each sample
    while(nCurrSample < nNumSamples)
    {
        fCurrSampledHeight = textureGrad(u_nmap, vTexCoord + vCurrOffset, dx, dy).a;
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

    vTexCoord += vCurrOffset;

    // normal mapping
    vec3 vNormal = texture(u_nmap, vTexCoord).rgb * 2.0 - 1.0;

    // convert normal from texture coords to OpenGL coords
    // texture coords are top-left based
    // OpenGL coords are bottom-left based
    vNormal.y *= -1;

    float fDistance = length(v_vTLight);
    vec3 vLight = normalize(v_vTLight);

    // attenuation
    float fDivisor = 1.0 +
                     fDistance * 0.05 +
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
        vSpecular *= texture(u_glossmap, vTexCoord).rgb;
    }

    // texture
    vec3 vTexColor = texture(u_texture, vTexCoord).rgb;

    // output
    o_vColor = vec4(vAmbient + vDiffuse * vTexColor + vSpecular, 1.0);
}
