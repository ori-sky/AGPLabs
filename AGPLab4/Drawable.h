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

#ifndef DRAWABLE_H
#define DRAWABLE_H

class Drawable
{
private:
    GLuint vao;
    GLuint vbo_vertex;
    GLuint vbo_normal;
    GLuint vbo_tangent;
    GLuint vbo_bitangent;
    GLuint vbo_texcoord;

    static GLuint make_buffer(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage)
    {
        GLuint id;
        glGenBuffers(1, &id);
        glBindBuffer(target, id);
        glBufferData(target, size, data, usage);
        return id;
    }

    static void make_vbo(GLsizeiptr size, const GLvoid *data, GLuint program_id,
                         const GLchar *attrib_name, GLint attrib_size, GLenum attrib_type)
    {
        Drawable::make_buffer(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        GLint attrib_id = glGetAttribLocation(program_id, attrib_name);
        glVertexAttribPointer(attrib_id, attrib_size, attrib_type, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(attrib_id);
    }
protected:
    virtual unsigned int make(glm::vec3 *vertices,
                              glm::vec3 *normals,
                              glm::vec3 *tangents,
                              glm::vec3 *bitangents,
                              glm::vec2 *texcoords) = 0;
public:
    void Init(GLuint program_id)
    {
        glm::vec3 *vertices   = NULL;
        glm::vec3 *normals    = NULL;
        glm::vec3 *tangents   = NULL;
        glm::vec3 *bitangents = NULL;
        glm::vec2 *texcoords  = NULL;

        unsigned int num = this->make(vertices, normals, tangents, bitangents, texcoords);

        glGenVertexArrays(1, &this->vao);
        glBindVertexArray(this->vao);

        make_vbo(num * sizeof(glm::vec3), vertices,   program_id, "a_vVertex",    3, GL_FLOAT);
        make_vbo(num * sizeof(glm::vec3), normals,    program_id, "a_vNormal",    3, GL_FLOAT);
        make_vbo(num * sizeof(glm::vec3), tangents,   program_id, "a_vTangent",   3, GL_FLOAT);
        make_vbo(num * sizeof(glm::vec3), bitangents, program_id, "a_vBitangent", 3, GL_FLOAT);
        make_vbo(num * sizeof(glm::vec2), texcoords,  program_id, "a_vTexCoord",  2, GL_FLOAT);

        delete[] vertices;
        delete[] normals;
        delete[] tangents;
        delete[] bitangents;
        delete[] texcoords;
    }

    virtual void Draw(void)
    {
        glBindVertexArray(this->vao);
    }
};

#endif
