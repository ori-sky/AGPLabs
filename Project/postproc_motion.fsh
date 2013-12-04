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
const float fBlurXY = 0.00015;
const float fBlurZ = 0.001;

void main(void)
{
    vec2 vTexCoord = v_vCoord;

    vec4 vColor = texture(u_sFBO, vTexCoord);

    // don't motion blur the left half of the screen
    if(vTexCoord.x < 0.5)
    {
        o_vColor = vColor;
        return;
    }

    if(vTexCoord.x - 0.5 < 0.005)
    {
        o_vColor = vec4(1, 1, 1, 1);
        return;
    }

    vec2 vVelocity = u_vVelocity.xy * fBlurXY;
    vVelocity.x += u_vVelocity.z * (vTexCoord.x - 0.5) * fBlurZ;
    vVelocity.y += u_vVelocity.z * (vTexCoord.y - 0.5) * fBlurZ;

    vTexCoord += vVelocity;
    for(int i=1; i<nMotionSamples; ++i, vTexCoord+=vVelocity)
    {
        vColor += texture(u_sFBO, vTexCoord);
    }

    o_vColor = vColor / nMotionSamples;
}
