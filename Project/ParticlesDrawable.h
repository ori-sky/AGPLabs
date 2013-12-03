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

#ifndef PARTICLESDRAWABLE_H
#define PARTICLESDRAWABLE_H

#include "Drawable.h"

class ParticlesDrawable : public Drawable
{
private:
    unsigned int num;

    GLuint vbo_point_sizes;

    GLfloat *point_sizes;
protected:
    virtual unsigned int Make(glm::vec3 **vertices,
                              glm::vec3 **normals,
                              glm::vec3 **tangents,
                              glm::vec3 **bitangents,
                              glm::vec2 **texcoords)
    {
        *vertices   = new glm::vec3[num];
        *normals    = new glm::vec3[num];
        *tangents   = new glm::vec3[num];
        *bitangents = new glm::vec3[num];
        *texcoords  = new glm::vec2[num];

        memset(*vertices,   0, num * sizeof(glm::vec3));
        memset(*normals,    0, num * sizeof(glm::vec3));
        memset(*tangents,   0, num * sizeof(glm::vec3));
        memset(*bitangents, 0, num * sizeof(glm::vec3));
        memset(*texcoords,  0, num * sizeof(glm::vec2));

        for(unsigned int i=0; i<num; ++i)
        {
            (*vertices)[i].x = rand() / (float)RAND_MAX * 2 - 1;
            (*vertices)[i].y = rand() / (float)RAND_MAX * 2 - 1;
            (*vertices)[i].z = rand() / (float)RAND_MAX * 2 - 1;
        }

        return num;
    }
public:
    ParticlesDrawable(unsigned int num) : Drawable(GL_DYNAMIC_DRAW), num(num) {}

#define GAME_DOMAIN "ParticlesDrawable::Draw"
    void Init(GLuint program_id)
    {
        Drawable::Init(program_id);

        point_sizes = new GLfloat[num];

        for(unsigned int i=0; i<num; ++i)
        {
            point_sizes[i] = 10;
        }

        vbo_point_sizes = MakeVBO(num * sizeof(GLfloat), point_sizes, program_id, "a_fPointSize", 1, GL_FLOAT, usage);
    }
#undef GAME_DOMAIN

    void Update(void)
    {
        Bind();

        unsigned int times = rand() / (float)RAND_MAX * 10;

        for(unsigned int i=0; i<times; ++i)
        {
            unsigned int index = rand() / (float)RAND_MAX * num;
            point_sizes[index] += 4.0f;

            UpdateBuffer(vbo_point_sizes, GL_ARRAY_BUFFER, index * sizeof(GLfloat), 1 * sizeof(GLfloat), &point_sizes[index]);
        }
    }

#define GAME_DOMAIN "ParticlesDrawable::Draw"
    virtual void Draw(GLuint program_id, GLint u_matModelView, glm::mat4 matModelView)
    {
        Drawable::Draw(program_id, u_matModelView, matModelView);

        ASSERT_GL(GLint loc = glGetUniformLocation(program_id, "u_bPoints"))
        ASSERT_GL(glUniform1i(loc, 1))

        ASSERT_GL(glDrawArrays(GL_POINTS, 0, num))

        ASSERT_GL(glUniform1i(loc, 0))
    }
#undef GAME_DOMAIN
};

#endif