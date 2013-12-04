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

#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <vector>

#include "common.h"

#include "ResourceManager.h"
#include "TextureManager.h"
#include "LightingManager.h"
#include "CubeDrawable.h"
#include "ParticlesDrawable.h"

#define GAME_ATTRIB_VERTEX 0
#define GAME_ATTRIB_NORMAL 1
#define GAME_ATTRIB_TEXCOORD 2

class Game
{
protected:
    std::vector<SDL_Keycode> pressed_keys;
    
    SDL_Window *wnd;
    SDL_GLContext ctx;

    float width;
    float height;
    float aspect;

    GLuint program_id;

    GLuint tex;
    GLuint nmap;
    GLuint glossmap;

    GLuint fbo;
    GLuint tex_fbo;
    GLuint rbo_depth;
    GLuint vbo_fbo_vertices;

    GLuint program_postproc;
    GLuint loc_fbo_a_vCoord;
    GLuint loc_fbo_u_sFBO;
    GLuint loc_fbo_u_vVelocity;
    GLuint loc_fbo_u_bEnabled;

    GLuint fbo_shadow;

    glm::mat4 matIdentity;
    glm::mat4 camera;

    CubeDrawable cube_left;
    CubeDrawable cube_right;
    ParticlesDrawable particles;

    glm::vec3 velocity;
public:
    Game() : particles(1000) {}
    static Game * New(void) { return new Game(); }
    void PrintShaderError(GLint shader);

    bool running;
    
    bool InitSDL(void);
    bool InitGLEW(void);
    bool InitShaders(const char *v_path, const char *f_path, GLuint *program);
    bool DestroySDL(void);

    bool Init(void);
    bool HandleSDL(SDL_Event *e);
    bool Draw(void);
    bool Update(float seconds);
    bool Destroy(void);
    int Run(int argc = 0, const char **argv = NULL);
};

#endif
