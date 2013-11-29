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
    glm::vec3 vAmbient;
    glm::vec3 vDiffuse;
    glm::vec3 vSpecular;
    GLfloat fAttenuationConst;
    GLfloat fAttenuationLinear;
    GLfloat fAttenuationQuadratic;
    GLfloat fAttenuationCubic;
};

struct Light
{
    GLint bActive;
    GLint nType;
    glm::vec4 vPosition;
};

struct Material
{
    glm::vec3 vAmbient;
    glm::vec3 vDiffuse;
    glm::vec3 vSpecular;
    GLfloat fShininess;
    GLfloat fGlow;
};

class LightingManager
{
public:
    static LightType light_types[NUM_LIGHT_TYPES];
    static Light lights[NUM_LIGHTS];
    static Material materials[NUM_MATERIALS];

    static void Init(void);
};

#endif
