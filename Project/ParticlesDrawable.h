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

struct ParticleAttribute
{
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 jerk;
    glm::vec3 snap;
};

struct Particle
{
    long long time_remaining;

    ParticleAttribute position;
    ParticleAttribute position_min;
    ParticleAttribute position_max;

    ParticleAttribute size;
    ParticleAttribute size_min;
    ParticleAttribute size_max;
};

class ParticlesDrawable : public Drawable
{
private:
    unsigned int num;
    long long timestep;

    GLuint vbo_alive;
    GLuint vbo_point_size;

    GLint *alives;
    GLfloat *point_sizes;
    Particle *particles;
protected:
    virtual unsigned int Make(glm::vec3 **vertices, glm::vec3 **normals, glm::vec3 **tangents, glm::vec3 **bitangents,
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

        timestep = 10;

        alives = new GLint[num];
        point_sizes = new GLfloat[num];
        particles = new Particle[num];

        for(unsigned int i=0; i<num; ++i)
        {
            alives[i] = 0;
            point_sizes[i] = 10;
        }

        vbo_alive = MakeVBO(num * sizeof(GLint), alives, program_id, "a_bAlive", 1, GL_INT, usage);
        vbo_point_size = MakeVBO(num * sizeof(GLfloat), point_sizes, program_id, "a_fPointSize", 1, GL_FLOAT, usage);
    }
#undef GAME_DOMAIN

    void CreateParticle(unsigned int index)
    {
        vertices[index].x = rand() / (float)RAND_MAX * 2 - 1;
        vertices[index].y = rand() / (float)RAND_MAX * 2 - 1;
        vertices[index].z = rand() / (float)RAND_MAX * 2 - 1;

        alives[index] = 1;
        point_sizes[index] = 10;

        particles[index].time_remaining = 2000;

        Bind();
        UpdateBuffer(vbo_vertex, GL_ARRAY_BUFFER, index * sizeof(glm::vec3), sizeof(glm::vec3), &vertices[index]);
        UpdateBuffer(vbo_alive, GL_ARRAY_BUFFER, index * sizeof(glm::vec3), sizeof(GLint), &alives[index]);
        UpdateBuffer(vbo_point_size, GL_ARRAY_BUFFER, index * sizeof(GLfloat), sizeof(GLfloat), &point_sizes[index]);
    }

    void CreateParticle(void)
    {
        for(unsigned int i=0; i<num; ++i)
        {
            if(!alives[i])
            {
                CreateParticle(i);
                break;
            }
        }
    }

    void Update(long long ms)
    {
        static long long accumulator = 0;
        accumulator += ms;

        for(;accumulator>=timestep; accumulator-=timestep)
        {
            CreateParticle();
        }

        for(unsigned int i=0; i<num; ++i)
        {
            if(!alives[i]) continue;

            if((particles[i].time_remaining -= ms) <= 0)
            {
                alives[i] = 0;
                continue;
            }

            vertices[i].y += rand() / (float)RAND_MAX * 0.002f;
            point_sizes[i] += rand() / (float)RAND_MAX * 0.1f;
        }

        Bind();
        UpdateBuffer(vbo_vertex, GL_ARRAY_BUFFER, 0, num * sizeof(glm::vec3), vertices);
        UpdateBuffer(vbo_alive, GL_ARRAY_BUFFER, 0, num * sizeof(GLint), alives);
        UpdateBuffer(vbo_point_size, GL_ARRAY_BUFFER, 0, num * sizeof(GLfloat), point_sizes);
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
