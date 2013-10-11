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

#ifndef BLOCK_H
#define BLOCK_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_precision.hpp>

#include "glcommon.h"
#include "Object.h"

#define BLOCK_ATTRIB_VERTEX 0
#define BLOCK_ATTRIB_NORMAL 1

class Block : public Object<Block>
{
protected:
    GLuint vao;
    GLuint vbo_vertex;
    GLuint vbo_normal;

    glm::i8vec4 vertices[36];
    glm::i8vec4 normals[36];

    glm::vec3 position;
public:
    Block(void);

    glm::vec3 GetPosition(void) { return this->position; }
    glm::vec3 * GetPositionRef(void) { return &this->position; }
    float GetPositionX(void) { return this->position.x; }
    float GetPositionY(void) { return this->position.y; }
    float GetPositionZ(void) { return this->position.z; }

    void SetPosition(glm::vec3 position) { this->position = position; }
    void SetPosition(float x, float y, float z) { this->position = glm::vec3(x, y, z); }
    void SetPositionX(float x) { this->position.x = x; }
    void SetPositionY(float y) { this->position.y = y; }
    void SetPositionZ(float z) { this->position.z = z; }

    bool Init(void);
    bool Draw(glm::mat4 matModelView, GLint u_matModelView);
};

#endif
