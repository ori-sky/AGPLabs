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
            (*vertices)[i].x = arc4random_uniform(1000) / 500.0f - 1;
            (*vertices)[i].y = arc4random_uniform(1000) / 500.0f - 1;
            (*vertices)[i].z = arc4random_uniform(1000) / 500.0f - 1;
        }

        return num;
    }
public:
    ParticlesDrawable(unsigned int num) : Drawable(GL_DYNAMIC_DRAW), num(num) {}

    virtual void Draw(GLuint program_id, GLint u_matModelView, glm::mat4 matModelView)
    {
        Drawable::Draw(program_id, u_matModelView, matModelView);

        GLenum err;

        GLint loc = glGetUniformLocation(program_id, "u_bPoints");
        glUniform1i(loc, 1);

        glDrawArrays(GL_POINTS, 0, num);
        if((err = glGetError()) != GL_NO_ERROR) fprintf(stderr, "ParticlesDrawable::Draw => glDrawArrays: error: 0x%x\n", err);

        glUniform1i(loc, 0);
    }
};

#endif
