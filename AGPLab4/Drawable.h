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
        GLenum err;
        GLuint id;

        glGenBuffers(1, &id);
        if((err = glGetError()) != GL_NO_ERROR) fprintf(stderr, "Drawable::make_buffer => glGenBuffers: error: 0x%x\n", err);

        glBindBuffer(target, id);
        if((err = glGetError()) != GL_NO_ERROR) fprintf(stderr, "Drawable::make_buffer => glBindBuffer: error: 0x%x\n", err);

        glBufferData(target, size, data, usage);
        if((err = glGetError()) != GL_NO_ERROR) fprintf(stderr, "Drawable::make_buffer => glBufferData: error: 0x%x\n", err);

        return id;
    }

    static void make_vbo(GLsizeiptr size, const GLvoid *data, GLuint program_id,
                         const GLchar *attrib_name, GLint attrib_size, GLenum attrib_type)
    {
        GLenum err;

        Drawable::make_buffer(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

        GLint attrib_id = glGetAttribLocation(program_id, attrib_name);
        if((err = glGetError()) != GL_NO_ERROR) fprintf(stderr, "Drawable::make_vbo => glGetAttribLocation: error: 0x%x\n", err);
        if(attrib_id < 0) fprintf(stderr, "Drawable::make_vbo => glGetAttribLocation: attribute is not active in program\n");

        glVertexAttribPointer(attrib_id, attrib_size, attrib_type, GL_FALSE, 0, NULL);
        if((err = glGetError()) != GL_NO_ERROR) fprintf(stderr, "Drawable::make_vbo => glVertexAttribPointer: error: 0x%x\n", err);

        glEnableVertexAttribArray(attrib_id);
        if((err = glGetError()) != GL_NO_ERROR) fprintf(stderr, "Drawable::make_vbo => glEnableVertexAttribArray: error: 0x%x\n", err);
    }
protected:
    virtual unsigned int Make(glm::vec3 *vertices,
                              glm::vec3 *normals,
                              glm::vec3 *tangents,
                              glm::vec3 *bitangents,
                              glm::vec2 *texcoords) = 0;
public:
    void Init(GLuint program_id)
    {
        GLenum err;

        glm::vec3 *vertices   = NULL;
        glm::vec3 *normals    = NULL;
        glm::vec3 *tangents   = NULL;
        glm::vec3 *bitangents = NULL;
        glm::vec2 *texcoords  = NULL;

        // clear errors
        while(glGetError() != GL_NO_ERROR);

        unsigned int num = this->Make(vertices, normals, tangents, bitangents, texcoords);

        glGenVertexArrays(1, &this->vao);
        if((err = glGetError()) != GL_NO_ERROR) fprintf(stderr, "Drawable::Init => glGenVertexArrays: error: 0x%x\n", err);

        glBindVertexArray(this->vao);
        if((err = glGetError()) != GL_NO_ERROR) fprintf(stderr, "Drawable::Init => glBindVertexArray: error: 0x%x\n", err);

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
        GLenum err;

        glBindVertexArray(this->vao);
        if((err = glGetError()) != GL_NO_ERROR) fprintf(stderr, "Drawable::Draw => glBindVertexArray: error: 0x%x\n", err);
    }
};

#endif
