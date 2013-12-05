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

template <typename T>
struct BoundedValue
{
    T value;
    T min;
    T max;

    void Clamp(void);
};

template <typename T>
struct ParticleAttribute
{
    BoundedValue<T> velocity;
    BoundedValue<T> acceleration;
    BoundedValue<T> jerk;
    BoundedValue<T> snap;

    void Update(long long ms)
    {
        jerk.value += snap.value * (float)ms;
        jerk.Clamp();

        acceleration.value += jerk.value * (float)ms;
        acceleration.Clamp();

        velocity.value += acceleration.value * (float)ms;
        velocity.Clamp();
    }
};

struct Particle
{
    long long time_remaining;

    ParticleAttribute<glm::vec3> position;
    ParticleAttribute<glm::vec3> rotation;
    ParticleAttribute<glm::vec3> offset;
    ParticleAttribute<GLfloat> size;

    void Update(long long ms)
    {
        position.Update(ms);
        rotation.Update(ms);
        offset.Update(ms);
        size.Update(ms);
    }
};

class ParticlesDrawable : public Drawable
{
private:
    unsigned int num;
    long long timestep;

    GLuint vbo_alive;
    GLuint vbo_point_size;
    GLuint vbo_rotation;
    GLuint vbo_offset;

    GLint *alives;
    GLfloat *point_sizes;
    glm::vec3 *rotations;
    glm::vec3 *offsets;

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

        timestep = 10000.0f / num;

        alives = new GLint[num];
        point_sizes = new GLfloat[num];
        rotations = new glm::vec3[num];
        offsets = new glm::vec3[num];

        particles = new Particle[num];

        memset(alives, 0, num * sizeof(GLint));

        vbo_alive = MakeVBO(num * sizeof(GLint), alives, program_id, "a_bAlive", 1, GL_INT, usage);
        vbo_point_size = MakeVBO(num * sizeof(GLfloat), point_sizes, program_id, "a_fPointSize", 1, GL_FLOAT, usage);
        vbo_rotation = MakeVBO(num * sizeof(glm::vec3), rotations, program_id, "a_vRotation", 3, GL_FLOAT, usage);
        vbo_offset = MakeVBO(num * sizeof(glm::vec3), offsets, program_id, "a_vOffset", 3, GL_FLOAT, usage);
    }
#undef GAME_DOMAIN

    void CreateParticle(unsigned int index)
    {
        //vertices[index].x = rand() / (float)RAND_MAX * 2 - 1;
        //vertices[index].y = rand() / (float)RAND_MAX * 2 - 1;
        //vertices[index].z = rand() / (float)RAND_MAX * 2 - 1;

        vertices[index].x = 0;
        vertices[index].y = -1;
        vertices[index].z = 0;

        rotations[index].x = 0;
        rotations[index].y = 3.14159265 * 2 * rand() / (float)RAND_MAX;
        rotations[index].z = 0;

        offsets[index].x = 0.1f * rand() / (float)RAND_MAX;
        offsets[index].y = 0;
        offsets[index].z = 0;

        alives[index] = 1;
        point_sizes[index] = 10;

        memset(&particles[index], 0, sizeof(Particle));

        particles[index].time_remaining = 5000;

        particles[index].position.velocity.value.y = 0.01f + 0.005f * rand() / (float)RAND_MAX;
        particles[index].position.velocity.min.y = -0.001f;
        particles[index].position.velocity.max.y = 1;
        particles[index].position.acceleration.value.y = -0.000005f;
        particles[index].position.acceleration.min.y = -1;
        particles[index].position.acceleration.max.y =  1;

        particles[index].size.velocity.value = 0.5f * rand() / (float)RAND_MAX;
        particles[index].size.velocity.min = -100;
        particles[index].size.velocity.max =  100;

        particles[index].rotation.velocity.value.y = 0.001f + 0.005f * rand() / (float)RAND_MAX;
        particles[index].rotation.velocity.min.y = -100;
        particles[index].rotation.velocity.max.y =  100;
        particles[index].rotation.acceleration.value.y = 0.000001f + 0.00001f * rand() / (float)RAND_MAX;
        particles[index].rotation.acceleration.min.y = -100;
        particles[index].rotation.acceleration.max.y =  100;

        particles[index].offset.velocity.value.x = 0.00001f + 0.01f * rand() / (float)RAND_MAX;
        particles[index].offset.velocity.min.x = 0;
        particles[index].offset.velocity.max.x = 1;

        Bind();
        UpdateBuffer(vbo_vertex, GL_ARRAY_BUFFER, index * sizeof(glm::vec3), sizeof(glm::vec3), &vertices[index]);
        UpdateBuffer(vbo_alive, GL_ARRAY_BUFFER, index * sizeof(GLint), sizeof(GLint), &alives[index]);
        UpdateBuffer(vbo_point_size, GL_ARRAY_BUFFER, index * sizeof(GLfloat), sizeof(GLfloat), &point_sizes[index]);
        UpdateBuffer(vbo_rotation, GL_ARRAY_BUFFER, index * sizeof(glm::vec3), sizeof(glm::vec3), &rotations[index]);
        UpdateBuffer(vbo_offset, GL_ARRAY_BUFFER, index * sizeof(glm::vec3), sizeof(glm::vec3), &offsets[index]);
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

        glm::mat3 matRotationY;
        matRotationY[0] = glm::vec3(0, 0, 0);
        matRotationY[1] = glm::vec3(0, 1, 0);
        matRotationY[2] = glm::vec3(0, 0, 0);


        for(unsigned int i=0; i<num; ++i)
        {
            if(!alives[i]) continue;

            if((particles[i].time_remaining -= ms) <= 0)
            {
                alives[i] = 0;
                LightingManager::lights[i + 5].bActive = 0;
                continue;
            }

            particles[i].Update(ms);

            vertices[i] += particles[i].position.velocity.value;
            point_sizes[i] += particles[i].size.velocity.value;
            rotations[i] += particles[i].rotation.velocity.value;
            offsets[i] += particles[i].offset.velocity.value;

            if(i < NUM_LIGHTS - 5)
            {
                // crazy optimization
                float c = cos(rotations[i].y);
                float s = sin(rotations[i].y);
                matRotationY[0].x =  c;
                matRotationY[0].z = -s;
                matRotationY[2].x =  s;
                matRotationY[2].z =  c;

                glm::vec3 vAbsolute = matRotationY * (vertices[i] + offsets[i]);
                LightingManager::lights[i + 5].vPosition.x = vAbsolute.x;
                LightingManager::lights[i + 5].vPosition.y = vAbsolute.y;
                LightingManager::lights[i + 5].vPosition.z = vAbsolute.z;
                LightingManager::lights[i + 5].bActive = 1;
            }
        }

        Bind();
        UpdateBuffer(vbo_vertex, GL_ARRAY_BUFFER, 0, num * sizeof(glm::vec3), vertices);
        UpdateBuffer(vbo_alive, GL_ARRAY_BUFFER, 0, num * sizeof(GLint), alives);
        UpdateBuffer(vbo_point_size, GL_ARRAY_BUFFER, 0, num * sizeof(GLfloat), point_sizes);
        UpdateBuffer(vbo_rotation, GL_ARRAY_BUFFER, 0, num * sizeof(glm::vec3), rotations);
        UpdateBuffer(vbo_offset, GL_ARRAY_BUFFER, 0, num * sizeof(glm::vec3), offsets);
    }

#define GAME_DOMAIN "ParticlesDrawable::Draw"
    virtual void Draw(GLuint program_id, GLint u_matModelView, glm::mat4 matModelView)
    {
        Drawable::Draw(program_id, u_matModelView, matModelView);

        ASSERT_GL(GLint loc = glGetUniformLocation(program_id, "u_bPoints"))
        ASSERT_GL(glUniform1i(loc, 1))

        glDepthMask(GL_FALSE);
        ASSERT_GL(glDrawArrays(GL_POINTS, 0, num))
        glDepthMask(GL_TRUE);

        ASSERT_GL(glUniform1i(loc, 0))
    }
#undef GAME_DOMAIN
};

#endif
