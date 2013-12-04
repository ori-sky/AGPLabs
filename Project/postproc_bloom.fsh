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

uniform sampler2D u_sFBO;
uniform int u_bEnabled;

smooth in vec2 v_vCoord;

out vec4 o_vColor;

const float fThreshold = 0.3;
const float fBloomIntensity = 2.0;
const float fOriginalIntensity = 1.0;
const float fBloomSaturation = 0.7;
const float fOriginalSaturation = 1.1;

vec4 only_light(in vec4 vColor, in float fThreshold)
{
    return clamp((vColor - fThreshold) / (1 - fThreshold), 0.0, 1.0);
}

vec4 adjust_saturation(in vec4 vColor, in float fSaturation)
{
    float fGray = dot(vec3(vColor), vec3(0.3, 0.59, 0.11));
    return mix(vec4(fGray), vColor, vec4(fSaturation));
}

vec4 gaussian_blur(in sampler2D sTex, in vec2 vTexCoord, float fThreshold)
{
    vec2 vHorizCoords[14];
    vHorizCoords[ 0] = vTexCoord + vec2(-0.028, 0.0);
    vHorizCoords[ 1] = vTexCoord + vec2(-0.024, 0.0);
    vHorizCoords[ 2] = vTexCoord + vec2(-0.020, 0.0);
    vHorizCoords[ 3] = vTexCoord + vec2(-0.016, 0.0);
    vHorizCoords[ 4] = vTexCoord + vec2(-0.012, 0.0);
    vHorizCoords[ 5] = vTexCoord + vec2(-0.008, 0.0);
    vHorizCoords[ 6] = vTexCoord + vec2(-0.004, 0.0);
    vHorizCoords[ 7] = vTexCoord + vec2( 0.004, 0.0);
    vHorizCoords[ 8] = vTexCoord + vec2( 0.008, 0.0);
    vHorizCoords[ 9] = vTexCoord + vec2( 0.012, 0.0);
    vHorizCoords[10] = vTexCoord + vec2( 0.016, 0.0);
    vHorizCoords[11] = vTexCoord + vec2( 0.020, 0.0);
    vHorizCoords[12] = vTexCoord + vec2( 0.024, 0.0);
    vHorizCoords[13] = vTexCoord + vec2( 0.028, 0.0);

    vec2 vVertCoords[14];
    // TODO: make aspect ratio uniform so it is not hardcoded
    vVertCoords[ 0] = vTexCoord + vec2(0.0, -0.028 / 1.778);
    vVertCoords[ 1] = vTexCoord + vec2(0.0, -0.024 / 1.778);
    vVertCoords[ 2] = vTexCoord + vec2(0.0, -0.020 / 1.778);
    vVertCoords[ 3] = vTexCoord + vec2(0.0, -0.016 / 1.778);
    vVertCoords[ 4] = vTexCoord + vec2(0.0, -0.012 / 1.778);
    vVertCoords[ 5] = vTexCoord + vec2(0.0, -0.008 / 1.778);
    vVertCoords[ 6] = vTexCoord + vec2(0.0, -0.004 / 1.778);
    vVertCoords[ 7] = vTexCoord + vec2(0.0,  0.004 / 1.778);
    vVertCoords[ 8] = vTexCoord + vec2(0.0,  0.008 / 1.778);
    vVertCoords[ 9] = vTexCoord + vec2(0.0,  0.012 / 1.778);
    vVertCoords[10] = vTexCoord + vec2(0.0,  0.016 / 1.778);
    vVertCoords[11] = vTexCoord + vec2(0.0,  0.020 / 1.778);
    vVertCoords[12] = vTexCoord + vec2(0.0,  0.024 / 1.778);
    vVertCoords[13] = vTexCoord + vec2(0.0,  0.028 / 1.778);

    vec4 vHorizBlur = vec4(0.0);
    vHorizBlur += only_light(texture(sTex, vHorizCoords[ 0]), fThreshold) * 0.0044299121055113265;
    vHorizBlur += only_light(texture(sTex, vHorizCoords[ 1]), fThreshold) * 0.00895781211794;
    vHorizBlur += only_light(texture(sTex, vHorizCoords[ 2]), fThreshold) * 0.0215963866053;
    vHorizBlur += only_light(texture(sTex, vHorizCoords[ 3]), fThreshold) * 0.0443683338718;
    vHorizBlur += only_light(texture(sTex, vHorizCoords[ 4]), fThreshold) * 0.0776744219933;
    vHorizBlur += only_light(texture(sTex, vHorizCoords[ 5]), fThreshold) * 0.115876621105;
    vHorizBlur += only_light(texture(sTex, vHorizCoords[ 6]), fThreshold) * 0.147308056121;
    vHorizBlur += only_light(texture(sTex, vTexCoord       ), fThreshold) * 0.159576912161;
    vHorizBlur += only_light(texture(sTex, vHorizCoords[ 7]), fThreshold) * 0.147308056121;
    vHorizBlur += only_light(texture(sTex, vHorizCoords[ 8]), fThreshold) * 0.115876621105;
    vHorizBlur += only_light(texture(sTex, vHorizCoords[ 9]), fThreshold) * 0.0776744219933;
    vHorizBlur += only_light(texture(sTex, vHorizCoords[10]), fThreshold) * 0.0443683338718;
    vHorizBlur += only_light(texture(sTex, vHorizCoords[11]), fThreshold) * 0.0215963866053;
    vHorizBlur += only_light(texture(sTex, vHorizCoords[12]), fThreshold) * 0.00895781211794;
    vHorizBlur += only_light(texture(sTex, vHorizCoords[13]), fThreshold) * 0.0044299121055113265;

    vec4 vVertBlur = vec4(0.0);
    vVertBlur += only_light(texture(sTex, vVertCoords[ 0]), fThreshold) * 0.0044299121055113265;
    vVertBlur += only_light(texture(sTex, vVertCoords[ 1]), fThreshold) * 0.00895781211794;
    vVertBlur += only_light(texture(sTex, vVertCoords[ 2]), fThreshold) * 0.0215963866053;
    vVertBlur += only_light(texture(sTex, vVertCoords[ 3]), fThreshold) * 0.0443683338718;
    vVertBlur += only_light(texture(sTex, vVertCoords[ 4]), fThreshold) * 0.0776744219933;
    vVertBlur += only_light(texture(sTex, vVertCoords[ 5]), fThreshold) * 0.115876621105;
    vVertBlur += only_light(texture(sTex, vVertCoords[ 6]), fThreshold) * 0.147308056121;
    vVertBlur += only_light(texture(sTex, vTexCoord      ), fThreshold) * 0.159576912161;
    vVertBlur += only_light(texture(sTex, vVertCoords[ 7]), fThreshold) * 0.147308056121;
    vVertBlur += only_light(texture(sTex, vVertCoords[ 8]), fThreshold) * 0.115876621105;
    vVertBlur += only_light(texture(sTex, vVertCoords[ 9]), fThreshold) * 0.0776744219933;
    vVertBlur += only_light(texture(sTex, vVertCoords[10]), fThreshold) * 0.0443683338718;
    vVertBlur += only_light(texture(sTex, vVertCoords[11]), fThreshold) * 0.0215963866053;
    vVertBlur += only_light(texture(sTex, vVertCoords[12]), fThreshold) * 0.00895781211794;
    vVertBlur += only_light(texture(sTex, vVertCoords[13]), fThreshold) * 0.0044299121055113265;

    return vVertBlur;
}

void main(void)
{
    vec2 vTexCoord = v_vCoord;
    vec4 vColor = texture(u_sFBO, vTexCoord);

    if(u_bEnabled == 0)
    {
        o_vColor = vColor;
        return;
    }

    vec4 vLight = clamp((vColor - fThreshold) / (1 - fThreshold), 0.0, 1.0);
    vec4 vBlurred = gaussian_blur(u_sFBO, vTexCoord, fThreshold);
    o_vColor = vBlurred;
    //return;

    vec4 vBloomColor = adjust_saturation(vBlurred, fBloomSaturation) * fBloomIntensity;
    vec4 vOriginalColor = adjust_saturation(vColor, fOriginalSaturation) * fOriginalIntensity;

    // darken original around areas of bloom to avoid burn-out
    vOriginalColor *= (1 - clamp(vBloomColor, 0.0, 1.0));

    o_vColor = vOriginalColor + vBloomColor;
}
