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

#ifndef CHUNK_H
#define CHUNK_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_precision.hpp>

#include "glcommon.h"
#include "Object.h"
#include "Block.h"

#define CHUNK_X 64
#define CHUNK_Y 64
#define CHUNK_Z 64

class Chunk : Object<Chunk>
{
protected:
    Block *blocks;
    glm::vec3 position;
public:
    Chunk(glm::vec3 position = glm::vec3(0.0)) : position(position)
    {
        this->blocks = new Block[CHUNK_X * CHUNK_Y * CHUNK_Z];
    }

    ~Chunk()
    {
        delete[] blocks;
    }

    inline Block * Get(unsigned long i) const
    {
        return &this->blocks[i];
    }

    inline Block * Get(unsigned long x, unsigned long y, unsigned long z) const
    {
        return Get(x + y * CHUNK_X + z * CHUNK_X * CHUNK_Y);
    }

    inline bool Init(void)
    {
        return true;
    }

    inline bool Draw(void)
    {
        return true;
    }
};

#endif
