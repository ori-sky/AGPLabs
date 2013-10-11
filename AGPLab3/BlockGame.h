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

#ifndef BLOCKGAME_H
#define BLOCKGAME_H

#include <stdio.h>
#include <string.h>
#include <vector>

#include <SDL.h>

#if defined(_WIN32) || defined(__linux__)
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_precision.hpp>

#include "ResourceManager.h"

#define BLOCKGAME_ATTRIB_VERTEX 0
#define BLOCKGAME_ATTRIB_NORMAL 1

class BlockGame
{
protected:
    std::vector<SDL_Keycode> pressed_keys;
    
    SDL_Window *wnd;
    SDL_GLContext ctx;

    GLuint program_id;

    GLuint vao;
    GLuint vbo_vertex;
    GLuint vbo_normal;
    GLuint ibo;

    glm::mat4 matIdentity;
    glm::mat4 camera;
    float obj_rotation;
public:
    static BlockGame * New(void) { return new BlockGame(); }
    void PrintShaderError(GLint shader);

    bool running;
    
    bool InitSDL(void);
    bool InitGLEW(void);
    bool InitShaders(const char *v_path, const char *f_path);
    bool DestroySDL(void);

    bool Init(void);
    bool HandleSDL(SDL_Event *e);
    bool Draw(void);
    bool Update(float seconds);
    bool Destroy(void);
    int Run(int argc = 0, const char **argv = NULL);
};

#endif
