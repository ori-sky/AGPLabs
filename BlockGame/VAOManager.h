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

#ifndef VAOMANAGER_H
#define VAOMANAGER_H

#include <vector>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_precision.hpp>

#include "glcommon.h"

struct VAO
{
    glm::i8vec4 *vertices;
    GLuint id;
};

class VAOManager
{
public:
    std::vector<VAO *> vaos;

    inline VAO * GetVAOFor(glm::i8vec4 *vertices)
    {
        for(std::vector<VAO *>::iterator it=this->vaos.begin(); it!=this->vaos.end(); ++it)
        {
            bool match = true;
            for(unsigned char v=0; v<36; ++v)
            {
                if((*it)->vertices[v] != vertices[v])
                {
                    match = false;
                    break;
                }
            }

            if(match) return (*it);
        }

        VAO *vao = new VAO();
        vao->vertices = vertices;
        glGenVertexArrays(1, &vao->id);

        this->vaos.push_back(vao);
        return vao;
    }
};

#endif
