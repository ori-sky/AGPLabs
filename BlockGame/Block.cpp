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

#include "Block.h"

Block::Block(void)
{
    // left
    this->vertices[0]  = glm::i8vec4(-1,  0,  0, 0);
    this->vertices[1]  = glm::i8vec4(-1, -1, -1, 0);
    this->vertices[2]  = glm::i8vec4(-1, -1,  1, 0);
    this->vertices[3]  = glm::i8vec4(-1,  1,  1, 0);
    this->vertices[4]  = glm::i8vec4(-1,  1, -1, 0);
    this->vertices[5]  = glm::i8vec4(-1, -1, -1, 0);

    // right
    this->vertices[6]  = glm::i8vec4( 1,  0,  0, 0);
    this->vertices[7]  = glm::i8vec4( 1, -1, -1, 0);
    this->vertices[8]  = glm::i8vec4( 1,  1, -1, 0);
    this->vertices[9]  = glm::i8vec4( 1,  1,  1, 0);
    this->vertices[10] = glm::i8vec4( 1, -1,  1, 0);
    this->vertices[11] = glm::i8vec4( 1, -1, -1, 0);

    // bottom
    this->vertices[12] = glm::i8vec4( 0, -1,  0, 0);
    this->vertices[13] = glm::i8vec4(-1, -1, -1, 0);
    this->vertices[14] = glm::i8vec4( 1, -1, -1, 0);
    this->vertices[15] = glm::i8vec4( 1, -1,  1, 0);
    this->vertices[16] = glm::i8vec4(-1, -1,  1, 0);
    this->vertices[17] = glm::i8vec4(-1, -1, -1, 0);

    // top
    this->vertices[18] = glm::i8vec4( 0,  1,  0, 0);
    this->vertices[19] = glm::i8vec4(-1,  1, -1, 0);
    this->vertices[20] = glm::i8vec4(-1,  1,  1, 0);
    this->vertices[21] = glm::i8vec4( 1,  1,  1, 0);
    this->vertices[22] = glm::i8vec4( 1,  1, -1, 0);
    this->vertices[23] = glm::i8vec4(-1,  1, -1, 0);

    // front
    this->vertices[24] = glm::i8vec4( 0,  0, -1, 0);
    this->vertices[25] = glm::i8vec4(-1, -1, -1, 0);
    this->vertices[26] = glm::i8vec4(-1,  1, -1, 0);
    this->vertices[27] = glm::i8vec4( 1,  1, -1, 0);
    this->vertices[28] = glm::i8vec4( 1, -1, -1, 0);
    this->vertices[29] = glm::i8vec4(-1, -1, -1, 0);

    // back
    this->vertices[30] = glm::i8vec4( 0,  0,  1, 0);
    this->vertices[31] = glm::i8vec4(-1, -1,  1, 0);
    this->vertices[32] = glm::i8vec4( 1, -1,  1, 0);
    this->vertices[33] = glm::i8vec4( 1,  1,  1, 0);
    this->vertices[34] = glm::i8vec4(-1,  1,  1, 0);
    this->vertices[35] = glm::i8vec4(-1, -1,  1, 0);

    // left
    this->normals[0]   = glm::i8vec4(-1,  0,  0, 0);
    this->normals[1]   = glm::i8vec4(-1,  0,  0, 0);
    this->normals[2]   = glm::i8vec4(-1,  0,  0, 0);
    this->normals[3]   = glm::i8vec4(-1,  0,  0, 0);
    this->normals[4]   = glm::i8vec4(-1,  0,  0, 0);
    this->normals[5]   = glm::i8vec4(-1,  0,  0, 0);

    // right
    this->normals[6]   = glm::i8vec4( 1,  0,  0, 0);
    this->normals[7]   = glm::i8vec4( 1,  0,  0, 0);
    this->normals[8]   = glm::i8vec4( 1,  0,  0, 0);
    this->normals[9]   = glm::i8vec4( 1,  0,  0, 0);
    this->normals[10]  = glm::i8vec4( 1,  0,  0, 0);
    this->normals[11]  = glm::i8vec4( 1,  0,  0, 0);

    // bottom
    this->normals[12]  = glm::i8vec4( 0, -1,  0, 0);
    this->normals[13]  = glm::i8vec4( 0, -1,  0, 0);
    this->normals[14]  = glm::i8vec4( 0, -1,  0, 0);
    this->normals[15]  = glm::i8vec4( 0, -1,  0, 0);
    this->normals[16]  = glm::i8vec4( 0, -1,  0, 0);
    this->normals[17]  = glm::i8vec4( 0, -1,  0, 0);

    // top
    this->normals[18]  = glm::i8vec4( 0,  1,  0, 0);
    this->normals[19]  = glm::i8vec4( 0,  1,  0, 0);
    this->normals[20]  = glm::i8vec4( 0,  1,  0, 0);
    this->normals[21]  = glm::i8vec4( 0,  1,  0, 0);
    this->normals[22]  = glm::i8vec4( 0,  1,  0, 0);
    this->normals[23]  = glm::i8vec4( 0,  1,  0, 0);

    // front
    this->normals[24]  = glm::i8vec4( 0,  0, -1, 0);
    this->normals[25]  = glm::i8vec4( 0,  0, -1, 0);
    this->normals[26]  = glm::i8vec4( 0,  0, -1, 0);
    this->normals[27]  = glm::i8vec4( 0,  0, -1, 0);
    this->normals[28]  = glm::i8vec4( 0,  0, -1, 0);
    this->normals[29]  = glm::i8vec4( 0,  0, -1, 0);

    // back
    this->normals[30]  = glm::i8vec4( 0,  0,  1, 0);
    this->normals[31]  = glm::i8vec4( 0,  0,  1, 0);
    this->normals[32]  = glm::i8vec4( 0,  0,  1, 0);
    this->normals[33]  = glm::i8vec4( 0,  0,  1, 0);
    this->normals[34]  = glm::i8vec4( 0,  0,  1, 0);
    this->normals[35]  = glm::i8vec4( 0,  0,  1, 0);
}

bool Block::Init(void)
{
    return true;
}

bool Block::Draw(void)
{
    return true;
}
