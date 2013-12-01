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

#ifndef LIGHTINGMANAGER_H
#define LIGHTINGMANAGER_H

#include "common.h"

#define NUM_LIGHT_TYPES 16
#define NUM_LIGHTS 16
#define NUM_MATERIALS 64

struct LightType
{
    glm::vec4 vAmbient;
    glm::vec4 vDiffuse;
    glm::vec4 vSpecular;
    GLfloat fAttenuationConst;
    GLfloat fAttenuationLinear;
    GLfloat fAttenuationQuadratic;
    GLint nReserved1;
};

struct Light
{
    glm::vec4 vPosition;
    GLint nType;
    GLint bActive;
    GLint nReserved1;
    GLint nReserved2;
};

struct Material
{
    glm::vec4 vAmbient;
    glm::vec4 vDiffuse;
    glm::vec4 vSpecular;
    GLfloat fShininess;
    GLfloat fGlow;
};

class LightingManager
{
public:
    static LightType light_types[NUM_LIGHT_TYPES];
    static Light lights[NUM_LIGHTS];
    static Material materials[NUM_MATERIALS];

    static void UploadLightType(GLuint program_id, unsigned int index);
    static void UploadLight(GLuint program_id, unsigned int index);
    static void UploadMaterial(GLuint program_id, unsigned int index);

    static inline void MakeLightType(unsigned int index,
                                     glm::vec4 vAmbient, glm::vec4 vDiffuse, glm::vec4 vSpecular,
                                     GLfloat fAConst, GLfloat fALinear, GLfloat fAQuad)
    {
        light_types[index].vAmbient = vAmbient;
        light_types[index].vDiffuse = vDiffuse;
        light_types[index].vSpecular = vSpecular;
        light_types[index].fAttenuationConst = fAConst;
        light_types[index].fAttenuationLinear = fALinear;
        light_types[index].fAttenuationQuadratic = fAQuad;
    }

    static inline void MakeLight(unsigned int index, GLboolean bActive, GLuint nType, glm::vec4 vPos)
    {
        lights[index].vPosition = vPos;
        lights[index].nType = nType;
        lights[index].bActive = bActive;
    }

    static inline void MakeMaterial(unsigned int index,
                                    glm::vec4 vAmbient, glm::vec4 vDiffuse, glm::vec4 vSpecular,
                                    GLfloat fShininess, GLfloat fGlow)
    {
        materials[index].vAmbient = vAmbient;
        materials[index].vDiffuse = vDiffuse;
        materials[index].vSpecular = vSpecular;
        materials[index].fShininess = fShininess;
        materials[index].fGlow = fGlow;
    }

    static inline void Init(void)
    {
        for(unsigned int i=0; i<NUM_LIGHT_TYPES; ++i)
        {
            MakeLightType(i, glm::vec4(0,0,0,1), glm::vec4(1,1,1,1), glm::vec4(1,1,1,1), 1, 0, 0.2f);
        }

        for(unsigned int i=0; i<NUM_LIGHTS; ++i)
        {
            MakeLight(i, 0, 0, glm::vec4(0, 0, 0, 1));
        }

        for(unsigned int i=0; i<NUM_MATERIALS; ++i)
        {
            MakeMaterial(i, glm::vec4(0,0,0,1), glm::vec4(1,1,1,1), glm::vec4(1,1,1,1), 64, 0);
        }
    }

    static inline void UploadLightTypes(GLuint program_id)
    {
        for(unsigned int i=0; i<NUM_LIGHT_TYPES; ++i)
        {
            UploadLightType(program_id, i);
        }
    }

    static inline void UploadLights(GLuint program_id)
    {
        for(unsigned int i=0; i<NUM_LIGHTS; ++i)
        {
            UploadLight(program_id, i);
        }
    }

    static inline void UploadMaterials(GLuint program_id)
    {
        for(unsigned int i=0; i<NUM_MATERIALS; ++i)
        {
            UploadMaterial(program_id, i);
        }
    }

    static inline void UploadAll(GLuint program_id)
    {
        UploadLightTypes(program_id);
        UploadLights(program_id);
        UploadMaterials(program_id);
    }
};

#endif
