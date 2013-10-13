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

#define BLOCK_SIZE 32

class NewBlock : public Object<NewBlock>
{
    // TODO: find a way to make this significantly smaller
    // maybe calculate surface normals in the Chunk
    // no need to keep the normal data stored
    // maybe only store the 14 vertices too

    glm::i8vec4 vertices[36];
    glm::i8vec4 normals[36];
};

class Block : public Object<Block>
{
protected:
    GLuint vao;
    GLuint vbo_vertex;
    GLuint vbo_normal;

    glm::i8vec4 vertices[36];
    glm::i8vec4 normals[36];

    glm::vec3 position;
    glm::mat4 mat;
public:
    Block(void);

    inline GLuint GetVAO(void)
    {
        return this->vao;
    }
    inline glm::i8vec4 * GetVertices(void)
    {
        return this->vertices;
    }

    inline glm::i8vec4 * GetNormals(void)
    {
        return this->normals;
    }

    inline glm::vec3 GetPosition(void) { return this->position; }
    inline glm::vec3 * GetPositionRef(void) { return &this->position; }
    inline float GetPositionX(void) { return this->position.x; }
    inline float GetPositionY(void) { return this->position.y; }
    inline float GetPositionZ(void) { return this->position.z; }

    inline void UpdateMatrix(void)
    {
        this->mat = glm::mat4(1.0f);
        this->mat = glm::translate(this->mat, this->position);
    }

    inline void SetPosition(glm::vec3 position)
    {
        this->position = position;
        this->UpdateMatrix();
    }
    inline void SetPosition(float x, float y, float z)
    {
        this->SetPosition(glm::vec3(x, y, z));
    }
    inline void SetPositionX(float x)
    {
        this->position.x = x;
        this->UpdateMatrix();
    }
    inline void SetPositionY(float y)
    {
        this->position.y = y;
        this->UpdateMatrix();
    }
    inline void SetPositionZ(float z)
    {
        this->position.z = z;
        this->UpdateMatrix();
    }

    bool Init(GLuint vao);

    inline bool Draw(GLint u_matModelView)
    {
        glUniformMatrix4fv(u_matModelView, 1, GL_FALSE, glm::value_ptr(this->mat));

        /*
        for(unsigned int i=0; i<6; ++i)
        {
            glDrawArrays(GL_TRIANGLE_FAN, 6 * i, 6);
        }
        */

        // this is faster
        GLint firsts[] = {0, 6, 12, 18, 24, 30};
        GLint counts[] = {6, 6, 6, 6, 6, 6};
        glMultiDrawArrays(GL_TRIANGLE_FAN, firsts, counts, sizeof(firsts) / sizeof(GLint));

        return true;
    }
};

#endif
