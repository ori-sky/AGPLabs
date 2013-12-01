/*
 *  Copyright 2013 David Farrell
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#version 150
precision highp float;

const int NUM_LIGHT_TYPES = 16;
const int NUM_LIGHTS = 16;
const int NUM_MATERIALS = 64;

struct LightType
{
    vec4 vAmbient;
    vec4 vDiffuse;
    vec4 vSpecular;
    float fAttenuationConst;
    float fAttenuationLinear;
    float fAttenuationQuadratic;
    int nReserved1;
};

struct Light
{
    vec4 vPosition;     // w = 0 means directional light
    int nType;
    int bActive;
    int nReserved1;
    int nReserved2;
};

struct Material
{
    vec4 vAmbient;
    vec4 vDiffuse;
    vec4 vSpecular;
    float fShininess;   // specular exponent
    float fGlow;        // luminescence
};

layout (std140) uniform LightTypesBlock
{
    LightType u_LightTypes[NUM_LIGHT_TYPES];
};

layout (std140) uniform LightsBlock
{
    Light u_Lights[NUM_LIGHTS];
};

layout (std140) uniform MaterialsBlock
{
    Material u_Materials[NUM_MATERIALS];
};

uniform sampler2D u_sDiffuse;
uniform sampler2D u_sNormalHeight;
uniform sampler2D u_sSpecular;

uniform mat4 u_matModelView;

smooth in vec3 v_vVertex;
smooth in vec3 v_vNormal;
smooth in vec2 v_vTexCoord;
smooth in vec3 v_vEye;

smooth in vec3 v_vTLight;
smooth in vec3 v_vTEye;
smooth in vec3 v_vTNormal;

smooth in mat3 v_matWorldToTangent;

out vec4 o_vColor;

vec2 parallax_occlusion_mapping(in sampler2D sMap, in vec2 vTexCoord,
                                in vec3 vEye, in vec3 vNormal,
                                in float fScale, in float fMaxSamples, in float fMinSamples)
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

vec3 normal_mapping(in sampler2D sMap, in vec2 vTexCoord)
{
    vec3 vNormal = texture(sMap, vTexCoord).rgb * 2.0 - 1.0;

    // convert normal from texture coords to OpenGL coords
    // texture coords are top-left based
    // OpenGL coords are bottom-left based
    vNormal.y *= -1;

    return vNormal;
}

void lighting(in vec3 vNormal, out vec3 vAmbient, out vec3 vDiffuse, out vec3 vSpecular)
{
    vAmbient = vec3(0);
    vDiffuse = vec3(0);
    vSpecular = vec3(0);

    for(int i=0; i<NUM_LIGHTS; ++i)
    {
        if(u_Lights[i].bActive == 0) continue;

        int type = u_Lights[i].nType;

        vAmbient += u_LightTypes[type].vAmbient.xyz;

        vec3 vLightPos = vec3(u_matModelView * vec4(u_Lights[i].vPosition.xyz, 1));
        vec3 vAux = v_matWorldToTangent * (vLightPos - v_vVertex);

        vec3 vLightDir = normalize(vAux);
        float fDistance = length(vAux);

        // attenuation
        float fDivisor = u_LightTypes[type].fAttenuationConst +
                         u_LightTypes[type].fAttenuationLinear * fDistance +
                         u_LightTypes[type].fAttenuationQuadratic * pow(fDistance, 2.0);
        float fAttenuation = 1.0 / fDivisor;

        float fNDotL = max(0.0, dot(vNormal, vLightDir));
        vDiffuse += u_LightTypes[type].vDiffuse.xyz * fNDotL * fAttenuation;

        if(fNDotL > 0.0)
        {
            vec3 vHalf = normalize(vLightDir + normalize(v_vTEye));
            float fNDotH = max(0.0, dot(vNormal, vHalf));
            vSpecular += u_LightTypes[type].vSpecular.xyz * pow(fNDotH, 16.0) * fAttenuation;
        }
    }
}

void main(void)
{
    // parallax occlusion mapping
    vec2 vTexCoord = parallax_occlusion_mapping(u_sNormalHeight, v_vTexCoord, v_vTEye, v_vTNormal, 0.15, 64, 16);

    // normal mapping
    vec3 vNormal = normal_mapping(u_sNormalHeight, vTexCoord);

    // lighting
    vec3 vAmbient, vDiffuse, vSpecular;
    lighting(vNormal, vAmbient, vDiffuse, vSpecular);

    vec3 vTexDiffuse = texture(u_sDiffuse, vTexCoord).rgb;
    vec3 vTexSpecular = texture(u_sSpecular, vTexCoord).rgb;

    vec3 vFinalColor = vAmbient +
                       vDiffuse * vTexDiffuse +
                       vSpecular * vTexSpecular;
    o_vColor = vec4(vFinalColor, 1.0);
}
