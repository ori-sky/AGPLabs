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
uniform vec3 u_vVelocity;

smooth in vec2 v_vCoord;

out vec4 o_vColor;

const int nMotionSamples = 16;
const float fBlurXY = 0.0005;
const float fBlurZ = 0.001;

void main(void)
{
    vec2 vTexCoord = v_vCoord;

    vec2 vVelocity = u_vVelocity.xy * 0.0005;

    //float fFromCenter = length(vTexCoord - vec2(0.5));
    vVelocity.x += u_vVelocity.z * (vTexCoord.x - 0.5) * fBlurZ;
    vVelocity.y += u_vVelocity.z * (vTexCoord.y - 0.5) * fBlurZ;

    vec4 vColor = texture(u_sFBO, vTexCoord);

    vTexCoord += vVelocity;
    for(int i=1; i<nMotionSamples; ++i, vTexCoord+=vVelocity)
    {
        vColor += texture(u_sFBO, vTexCoord);
    }

    o_vColor = vColor / nMotionSamples;
}
