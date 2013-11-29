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

void LightingManager::UploadLightType(GLuint program_id, unsigned int index)
{
    // clear error
    glGetError();

    char str[64];
    str[sizeof(str) - 1] = 0;

    GLint loc;

    snprintf(str, sizeof(str), "u_LightTypes[%u].vAmbient", index);
    loc = glGetUniformLocation(program_id, str);
    if(loc != -1) glProgramUniform3fv(program_id, loc, 1, glm::value_ptr(light_types[index].vAmbient));

    snprintf(str, sizeof(str), "u_LightTypes[%u].vDiffuse", index);
    loc = glGetUniformLocation(program_id, str);
    if(loc != -1) glProgramUniform3fv(program_id, loc, 1, glm::value_ptr(light_types[index].vDiffuse));

    snprintf(str, sizeof(str), "u_LightTypes[%u].vSpecular", index);
    loc = glGetUniformLocation(program_id, str);
    if(loc != -1) glProgramUniform3fv(program_id, loc, 1, glm::value_ptr(light_types[index].vSpecular));

    snprintf(str, sizeof(str), "u_LightTypes[%u].fAttenuationConst", index);
    loc = glGetUniformLocation(program_id, str);
    if(loc != -1) glProgramUniform1f(program_id, loc, light_types[index].fAttenuationConst);

    snprintf(str, sizeof(str), "u_LightTypes[%u].fAttenuationLinear", index);
    loc = glGetUniformLocation(program_id, str);
    if(loc != -1) glProgramUniform1f(program_id, loc, light_types[index].fAttenuationLinear);

    snprintf(str, sizeof(str), "u_LightTypes[%u].vfAttenuationQuadratic", index);
    loc = glGetUniformLocation(program_id, str);
    if(loc != -1) glProgramUniform1f(program_id, loc, light_types[index].fAttenuationQuadratic);

    snprintf(str, sizeof(str), "u_LightTypes[%u].fAttenuationCubic", index);
    loc = glGetUniformLocation(program_id, str);
    if(loc != -1) glProgramUniform1f(program_id, loc, light_types[index].fAttenuationCubic);

    GLenum err;
    if((err = glGetError()) != GL_NO_ERROR) fprintf(stderr, "UploadLightType: error: 0x%x\n", err);
}

void LightingManager::UploadLight(GLuint program_id, unsigned int index)
{
    // clear error
    glGetError();

    char str[64];
    str[sizeof(str) - 1] = 0;

    GLint loc;

    snprintf(str, sizeof(str), "u_Lights[%u].bActive", index);
    loc = glGetUniformLocation(program_id, str);
    if(loc != -1) glProgramUniform1i(program_id, loc, lights[index].bActive);

    snprintf(str, sizeof(str), "u_Lights[%u].nType", index);
    loc = glGetUniformLocation(program_id, str);
    if(loc != -1) glProgramUniform1i(program_id, loc, lights[index].nType);

    snprintf(str, sizeof(str), "u_Lights[%u].vPosition", index);
    loc = glGetUniformLocation(program_id, str);
    if(loc != -1) glProgramUniform4fv(program_id, loc + 2, 1, glm::value_ptr(lights[index].vPosition));

    GLenum err;
    if((err = glGetError()) != GL_NO_ERROR) fprintf(stderr, "UploadLight: error: 0x%x\n", err);
}

void LightingManager::UploadMaterial(GLuint program_id, unsigned int index)
{
    // clear error
    glGetError();

    char str[64];
    str[sizeof(str) - 1] = 0;

    GLint loc;

    snprintf(str, sizeof(str), "u_Materials[%u].vAmbient", index);
    loc = glGetUniformLocation(program_id, str);
    if(loc != -1) glProgramUniform3fv(program_id, loc, 1, glm::value_ptr(materials[index].vAmbient));

    snprintf(str, sizeof(str), "u_Materials[%u].vDiffuse", index);
    loc = glGetUniformLocation(program_id, str);
    if(loc != -1) glProgramUniform3fv(program_id, loc, 1, glm::value_ptr(materials[index].vDiffuse));

    snprintf(str, sizeof(str), "u_Materials[%u].vSpecular", index);
    loc = glGetUniformLocation(program_id, str);
    if(loc != -1) glProgramUniform3fv(program_id, loc, 1, glm::value_ptr(materials[index].vSpecular));

    snprintf(str, sizeof(str), "u_Materials[%u].fShininess", index);
    loc = glGetUniformLocation(program_id, str);
    if(loc != -1) glProgramUniform1f(program_id, loc, materials[index].fShininess);

    snprintf(str, sizeof(str), "u_Materials[%u].fGlow", index);
    loc = glGetUniformLocation(program_id, str);
    if(loc != -1) glProgramUniform1f(program_id, loc, materials[index].fGlow);

    GLenum err;
    if((err = glGetError()) != GL_NO_ERROR) fprintf(stderr, "UploadLight: error: 0x%x\n", err);
}
