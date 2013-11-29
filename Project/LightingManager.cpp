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

#include "LightingManager.h"

LightType LightingManager::light_types[NUM_LIGHT_TYPES];
Light LightingManager::lights[NUM_LIGHTS];
Material LightingManager::materials[NUM_MATERIALS];

void LightingManager::Init(void)
{
    for(int i=0; i<NUM_LIGHT_TYPES; ++i)
    {
        light_types[i].vAmbient = glm::vec3(0);
        LightingManager::light_types[i].vDiffuse = glm::vec3(1);
        LightingManager::light_types[i].vSpecular = glm::vec3(1);
        LightingManager::light_types[i].fAttenuationConst = 1;
        LightingManager::light_types[i].fAttenuationLinear = 0;
        LightingManager::light_types[i].fAttenuationQuadratic = 0.2f;
        LightingManager::light_types[i].fAttenuationCubic = 0;
    }

    for(int i=0; i<NUM_LIGHTS; ++i)
    {
        LightingManager::lights[i].bActive = 0;
        LightingManager::lights[i].nType = 0;
        LightingManager::lights[i].vPosition = glm::vec4(0, 0, 0, 1);
    }

    for(int i=0; i<NUM_MATERIALS; ++i)
    {
        LightingManager::materials[i].vAmbient = glm::vec3(0);
        LightingManager::materials[i].vDiffuse = glm::vec3(1);
        LightingManager::materials[i].vSpecular = glm::vec3(1);
        LightingManager::materials[i].fShininess = 64;
        LightingManager::materials[i].fGlow = 0;
    }
}
