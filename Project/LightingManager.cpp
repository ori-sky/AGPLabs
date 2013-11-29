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
    for(unsigned int i=0; i<NUM_LIGHT_TYPES; ++i)
    {
        light_types[i].vAmbient = glm::vec3(0);
        light_types[i].vDiffuse = glm::vec3(1);
        light_types[i].vSpecular = glm::vec3(1);
        light_types[i].fAttenuationConst = 1;
        light_types[i].fAttenuationLinear = 0;
        light_types[i].fAttenuationQuadratic = 0.2f;
        light_types[i].fAttenuationCubic = 0;
    }

    for(unsigned int i=0; i<NUM_LIGHTS; ++i)
    {
        lights[i].bActive = 0;
        lights[i].nType = 0;
        lights[i].vPosition = glm::vec4(0, 0, 0, 1);
    }

    for(unsigned int i=0; i<NUM_MATERIALS; ++i)
    {
        materials[i].vAmbient = glm::vec3(0);
        materials[i].vDiffuse = glm::vec3(1);
        materials[i].vSpecular = glm::vec3(1);
        materials[i].fShininess = 64;
        materials[i].fGlow = 0;
    }
}

void LightingManager::UploadLightType(GLuint program_id, unsigned int index)
{
    // build reference to first member of struct
    char str[64];
    str[sizeof(str) - 1] = 0;
    snprintf(str, sizeof(str), "u_LightTypes[%u].vAmbient", index);

    GLint loc = glGetUniformLocation(program_id, str);
    glProgramUniform3fv(program_id, loc + 0, 1, glm::value_ptr(light_types[index].vAmbient));
    glProgramUniform3fv(program_id, loc + 1, 1, glm::value_ptr(light_types[index].vDiffuse));
    glProgramUniform3fv(program_id, loc + 2, 1, glm::value_ptr(light_types[index].vSpecular));
    glProgramUniform1f(program_id, loc + 3, light_types[index].fAttenuationConst);
    glProgramUniform1f(program_id, loc + 4, light_types[index].fAttenuationLinear);
    glProgramUniform1f(program_id, loc + 5, light_types[index].fAttenuationQuadratic);
    glProgramUniform1f(program_id, loc + 6, light_types[index].fAttenuationCubic);
}

void LightingManager::UploadLight(GLuint program_id, unsigned int index)
{
    // build reference to first member of struct
    char str[64];
    str[sizeof(str) - 1] = 0;
    snprintf(str, sizeof(str), "u_Lights[%u].bActive", index);

    GLint loc = glGetUniformLocation(program_id, str);
    glProgramUniform1i(program_id, loc + 0, lights[index].bActive);
    glProgramUniform1i(program_id, loc + 1, lights[index].nType);
    glProgramUniform4fv(program_id, loc + 2, 1, glm::value_ptr(lights[index].vPosition));
}

void LightingManager::UploadMaterial(GLuint program_id, unsigned int index)
{
    // build reference to first member of struct
    char str[64];
    str[sizeof(str) - 1] = 0;
    snprintf(str, sizeof(str), "u_Materials[%u].vAmbient", index);

    GLint loc = glGetUniformLocation(program_id, str);
    glProgramUniform3fv(program_id, loc + 0, 1, glm::value_ptr(materials[index].vAmbient));
    glProgramUniform3fv(program_id, loc + 1, 1, glm::value_ptr(materials[index].vDiffuse));
    glProgramUniform3fv(program_id, loc + 2, 1, glm::value_ptr(materials[index].vSpecular));
    glProgramUniform1f(program_id, loc + 3, materials[index].fShininess);
    glProgramUniform1f(program_id, loc + 4, materials[index].fGlow);
}
