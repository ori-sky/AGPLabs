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

#ifndef CUBEDRAWABLE_H
#define CUBEDRAWABLE_H

#include "Drawable.h"

class CubeDrawable : public Drawable
{
protected:
    virtual unsigned int make(glm::vec3 *vertices,
                              glm::vec3 *normals,
                              glm::vec3 *tangents,
                              glm::vec3 *bitangents,
                              glm::vec2 *texcoords)
    {
        /* 4 vertices per face
         * 6 faces per cube
         */
        const unsigned int num = 24;

        vertices   = new glm::vec3[num];
        normals    = new glm::vec3[num];
        tangents   = new glm::vec3[num];
        bitangents = new glm::vec3[num];
        texcoords  = new glm::vec2[num];

        return num;
    }
};

#endif
