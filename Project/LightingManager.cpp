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

#define GAME_DOMAIN "LightingManager::UploadLightTypes"
void LightingManager::UploadLightTypes(GLuint program_id)
{
    GLuint binding = 1;
    ASSERT_GL(GLuint block_index = glGetUniformBlockIndex(program_id, "LightTypesBlock"))
    ASSERT_GL(glUniformBlockBinding(program_id, block_index, binding))

    GLuint buffer;
    ASSERT_GL(glGenBuffers(1, &buffer))
    ASSERT_GL(glBindBuffer(GL_UNIFORM_BUFFER, buffer))
    ASSERT_GL(glBufferData(GL_UNIFORM_BUFFER, sizeof(light_types), light_types, GL_DYNAMIC_DRAW))
    ASSERT_GL(glBindBufferBase(GL_UNIFORM_BUFFER, binding, buffer))
}
#undef GAME_DOMAIN

#define GAME_DOMAIN "LightingManager::UploadLights"
void LightingManager::UploadLights(GLuint program_id)
{
    GLuint binding = 2;
    ASSERT_GL(GLuint block_index = glGetUniformBlockIndex(program_id, "LightsBlock"))
    ASSERT_GL(glUniformBlockBinding(program_id, block_index, binding))

    GLuint buffer;
    ASSERT_GL(glGenBuffers(1, &buffer))
    ASSERT_GL(glBindBuffer(GL_UNIFORM_BUFFER, buffer))
    ASSERT_GL(glBufferData(GL_UNIFORM_BUFFER, sizeof(lights), lights, GL_DYNAMIC_DRAW))
    ASSERT_GL(glBindBufferBase(GL_UNIFORM_BUFFER, binding, buffer))
}
#undef GAME_DOMAIN

#define GAME_DOMAIN "LightingManager::UploadMaterials"
void LightingManager::UploadMaterials(GLuint program_id)
{
    GLuint binding = 3;
    ASSERT_GL(GLuint block_index = glGetUniformBlockIndex(program_id, "MaterialsBlock"))
    ASSERT_GL(glUniformBlockBinding(program_id, block_index, binding))

    GLuint buffer;
    ASSERT_GL(glGenBuffers(1, &buffer))
    ASSERT_GL(glBindBuffer(GL_UNIFORM_BUFFER, buffer))
    ASSERT_GL(glBufferData(GL_UNIFORM_BUFFER, sizeof(materials), materials, GL_DYNAMIC_DRAW))
    ASSERT_GL(glBindBufferBase(GL_UNIFORM_BUFFER, binding, buffer))
}
#undef GAME_DOMAIN

#define GAME_DOMAIN "LightingManager::SetMaterial"
void LightingManager::SetMaterial(GLuint program_id, GLint material_id)
{
    ASSERT_GL(GLint loc = glGetUniformLocation(program_id, "u_nMaterial"));
    ASSERT_GL(glUniform1i(loc, material_id));
}
#undef GAME_DOMAIN

#define GAME_DOMAIN "LightingManager::SetExposure"
void LightingManager::SetExposure(GLuint program_id, GLfloat exposure)
{
    ASSERT_GL(GLint loc = glGetUniformLocation(program_id, "u_fExposure"))
    ASSERT_GL(glUniform1f(loc, exposure))
}
#undef GAME_DOMAIN
