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
    virtual unsigned int Make(glm::vec3 **vertices,
                              glm::vec3 **normals,
                              glm::vec3 **tangents,
                              glm::vec3 **bitangents,
                              glm::vec2 **texcoords)
    {
        /* 4 vertices per face
         * 6 faces per cube
         */
        const unsigned int num = 24;

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

        // vertices

        // left
        (*vertices)[ 0] = glm::vec3(-1, -1, -1);
        (*vertices)[ 1] = glm::vec3(-1, -1,  1);
        (*vertices)[ 2] = glm::vec3(-1,  1, -1);
        (*vertices)[ 3] = glm::vec3(-1,  1,  1);

        // right
        (*vertices)[ 4] = glm::vec3( 1, -1, -1);
        (*vertices)[ 5] = glm::vec3( 1,  1, -1);
        (*vertices)[ 6] = glm::vec3( 1, -1,  1);
        (*vertices)[ 7] = glm::vec3( 1,  1,  1);

        // bottom
        (*vertices)[ 8] = glm::vec3(-1, -1, -1);
        (*vertices)[ 9] = glm::vec3( 1, -1, -1);
        (*vertices)[10] = glm::vec3(-1, -1,  1);
        (*vertices)[11] = glm::vec3( 1, -1,  1);

        // top
        (*vertices)[12] = glm::vec3(-1,  1, -1);
        (*vertices)[13] = glm::vec3(-1,  1,  1);
        (*vertices)[14] = glm::vec3( 1,  1, -1);
        (*vertices)[15] = glm::vec3( 1,  1,  1);

        // front
        (*vertices)[16] = glm::vec3(-1, -1, -1);
        (*vertices)[17] = glm::vec3(-1,  1, -1);
        (*vertices)[18] = glm::vec3( 1, -1, -1);
        (*vertices)[19] = glm::vec3( 1,  1, -1);

        // back
        (*vertices)[20] = glm::vec3(-1, -1,  1);
        (*vertices)[21] = glm::vec3( 1, -1,  1);
        (*vertices)[22] = glm::vec3(-1,  1,  1);
        (*vertices)[23] = glm::vec3( 1,  1,  1);

        // texcoords

        // left
        (*texcoords)[ 0] = glm::vec2(0, 0);
        (*texcoords)[ 1] = glm::vec2(0, 1);
        (*texcoords)[ 2] = glm::vec2(1, 0);
        (*texcoords)[ 3] = glm::vec2(1, 1);

        // right
        (*texcoords)[ 4] = glm::vec2(0, 0);
        (*texcoords)[ 5] = glm::vec2(0, 1);
        (*texcoords)[ 6] = glm::vec2(1, 0);
        (*texcoords)[ 7] = glm::vec2(1, 1);

        // bottom
        (*texcoords)[ 8] = glm::vec2(0, 0);
        (*texcoords)[ 9] = glm::vec2(0, 1);
        (*texcoords)[10] = glm::vec2(1, 0);
        (*texcoords)[11] = glm::vec2(1, 1);

        // top
        (*texcoords)[12] = glm::vec2(0, 0);
        (*texcoords)[13] = glm::vec2(0, 1);
        (*texcoords)[14] = glm::vec2(1, 0);
        (*texcoords)[15] = glm::vec2(1, 1);

        // front
        (*texcoords)[16] = glm::vec2(0, 0);
        (*texcoords)[17] = glm::vec2(0, 1);
        (*texcoords)[18] = glm::vec2(1, 0);
        (*texcoords)[19] = glm::vec2(1, 1);

        // back
        (*texcoords)[20] = glm::vec2(0, 0);
        (*texcoords)[21] = glm::vec2(0, 1);
        (*texcoords)[22] = glm::vec2(1, 0);
        (*texcoords)[23] = glm::vec2(1, 1);

        for(int i=0; i<24; i+=4)
        {
            glm::vec3 vU = (*vertices)[i + 1] - (*vertices)[i];
            glm::vec3 vV = (*vertices)[i + 2] - (*vertices)[i];

            glm::vec3 normal = glm::vec3(0);
            normal.x = vU.y * vV.z - vU.z * vV.y;
            normal.y = vU.z * vV.x - vU.x * vV.z;
            normal.z = vU.x * vV.y - vU.y * vV.x;

            for(int j=0; j<4; ++j) (*normals)[i + j] = normal;
        }

        return num;
    }
public:
    virtual void Draw(void)
    {
        GLenum err;

        Drawable::Draw();

        const GLint firsts[] = {0, 4, 8, 12, 16, 20};
        const GLint counts[] = {4, 4, 4, 4, 4, 4};

        glMultiDrawArrays(GL_TRIANGLE_STRIP, firsts, counts, sizeof(firsts) / sizeof(GLint));
        if((err = glGetError()) != GL_NO_ERROR) fprintf(stderr, "CubeDrawable::Draw => glMultiDrawArrays: error: 0x%x\n", err);
    }
};

#endif
