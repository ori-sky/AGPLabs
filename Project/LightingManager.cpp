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

void LightingManager::UploadLightTypes(GLuint program_id)
{
    // clear error
    glGetError();

    GLuint binding = 1;
    GLuint block_index = glGetUniformBlockIndex(program_id, "LightTypesBlock");
    glUniformBlockBinding(program_id, block_index, binding);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, buffer);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(light_types), light_types, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, binding, buffer);

    GLenum err;
    if((err = glGetError()) != GL_NO_ERROR) fprintf(stderr, "UploadLightTypes: error: 0x%x\n", err);
}

void LightingManager::UploadLights(GLuint program_id)
{
    // clear error
    glGetError();

    GLuint binding = 2;
    GLuint block_index = glGetUniformBlockIndex(program_id, "LightsBlock");
    glUniformBlockBinding(program_id, block_index, binding);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, buffer);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(lights), lights, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, binding, buffer);

    GLenum err;
    if((err = glGetError()) != GL_NO_ERROR) fprintf(stderr, "UploadLights: error: 0x%x\n", err);
}

void LightingManager::UploadMaterials(GLuint program_id)
{
    // clear error
    glGetError();

    GLuint binding = 3;
    GLuint block_index = glGetUniformBlockIndex(program_id, "MaterialsBlock");
    glUniformBlockBinding(program_id, block_index, binding);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, buffer);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(materials), materials, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, binding, buffer);

    GLenum err;
    if((err = glGetError()) != GL_NO_ERROR) fprintf(stderr, "UploadMaterials: error: 0x%x\n", err);
}
