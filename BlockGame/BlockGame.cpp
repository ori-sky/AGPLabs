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

#include "BlockGame.h"

bool BlockGame::InitSDL(void)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL_Init: error: %s\n", SDL_GetError());
        return false;
    }

    // create dummy window and context to test default options

    this->wnd = SDL_CreateWindow("", 0, 0, 0, 0, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
    if(!wnd)
    {
        fprintf(stderr, "SDL_CreateWindow: dummy error: %s\n", SDL_GetError());
        return false;
    }

    this->ctx = SDL_GL_CreateContext(wnd);
    if(!this->ctx)
    {
        fprintf(stderr, "SDL_GL_CreateContext: dummy error: %s\n", SDL_GetError());
        return false;
    }

    // debug info
    fprintf(stderr, "[DUMMY] OpenGL version: %s\n", glGetString(GL_VERSION));
    fprintf(stderr, "[DUMMY] GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // destroy dummy window and context
    SDL_GL_DeleteContext(this->ctx);
    SDL_DestroyWindow(this->wnd);

    // set attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    // create real window and context

    this->wnd = SDL_CreateWindow("AGPLab1",
                                 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 1280, 720,
                                 SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(!this->wnd)
    {
        fprintf(stderr, "SDL_CreateWindow: error: %s\n", SDL_GetError());
        return false;
    }

    this->ctx = SDL_GL_CreateContext(wnd);
    if(!this->ctx)
    {
        fprintf(stderr, "SDL_GL_CreateContext: error: %s\n", SDL_GetError());
        return false;
    }

    // debug info
    fprintf(stderr, "OpenGL version: %s\n", glGetString(GL_VERSION));
    fprintf(stderr, "GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // enable vsync
    SDL_GL_SetSwapInterval(1);

    return true;
}

bool BlockGame::InitGLEW(void)
{
#if defined(_WIN32) || defined(__linux__)
    glewExperimental = true;

    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
        fprintf(stderr, "glewInit: error\n");
        return 1;
    }
#endif

    return true;
}

bool BlockGame::InitShaders(const char *v_path, const char *f_path)
{
    long v_len, f_len;
    const char *v_src = ResourceManager::Load(v_path, &v_len);
    const char *f_src = ResourceManager::Load(f_path, &f_len);

    if(v_src == NULL)
    {
        fprintf(stderr, "::InitShaders: error: failed to load vertex shader\n");
        return false;
    }

    if(f_src == NULL)
    {
        fprintf(stderr, "::InitShaders: error: failed to load fragment shader\n");
        return false;
    }

    GLuint v_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint f_id = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(v_id, 1, &v_src, (GLint *)&v_len);
    glShaderSource(f_id, 1, &f_src, (GLint *)&f_len);

    GLint compile_status;

    glCompileShader(v_id);
    glGetShaderiv(v_id, GL_COMPILE_STATUS, &compile_status);
    if(!compile_status)
    {
        fprintf(stderr, "glCompileShader(v_id): error\n");
        //print_shader_error(v_id);
        return false;
    }

    glCompileShader(f_id);
    glGetShaderiv(f_id, GL_COMPILE_STATUS, &compile_status);
    if(!compile_status)
    {
        fprintf(stderr, "glCompileShader(f_id): error\n");
        //print_shader_error(f_id);
        return false;
    }

    this->program_id = glCreateProgram();
    glAttachShader(this->program_id, v_id);
    glAttachShader(this->program_id, f_id);

    glBindAttribLocation(this->program_id, BLOCKGAME_ATTRIB_VERTEX, "a_vVertex");
    //glBindAttribLocation(p_id, ATTRIB_COLOR, "a_vColor");
    //glBindAttribLocation(p_id, ATTRIB_NORMAL, "a_vNormal");
    //glBindAttribLocation(p_id, ATTRIB_TEXCOORD, "a_vTexCoord");

    glLinkProgram(this->program_id);
    glUseProgram(this->program_id);

    return true;
}

bool BlockGame::DestroySDL(void)
{
    SDL_DestroyWindow(this->wnd);
    SDL_Quit();
    return true;
}

bool BlockGame::Init(void)
{
    if(!this->InitSDL()) return false;
    if(!this->InitGLEW()) return false;
    if(!this->InitShaders("minimal.vsh", "minimal.fsh")) return false;

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    static glm::i8vec4 const vertices[] =
    {
        // left
        glm::i8vec4(-1,  0,  0, 0),
        glm::i8vec4(-1, -1, -1, 0),
        glm::i8vec4(-1,  1, -1, 0),
        glm::i8vec4(-1,  1,  1, 0),
        glm::i8vec4(-1, -1,  1, 0),
        glm::i8vec4(-1, -1, -1, 0),

        // right
        glm::i8vec4( 1,  0,  0, 0),
        glm::i8vec4( 1, -1, -1, 0),
        glm::i8vec4( 1, -1,  1, 0),
        glm::i8vec4( 1,  1,  1, 0),
        glm::i8vec4( 1,  1, -1, 0),
        glm::i8vec4( 1, -1, -1, 0),

        // bottom
        glm::i8vec4( 0, -1,  0, 0),
        glm::i8vec4(-1, -1, -1, 0),
        glm::i8vec4(-1, -1,  1, 0),
        glm::i8vec4( 1, -1,  1, 0),
        glm::i8vec4( 1, -1, -1, 0),
        glm::i8vec4(-1, -1, -1, 0),

        // top
        glm::i8vec4( 0,  1,  0, 0),
        glm::i8vec4(-1,  1, -1, 0),
        glm::i8vec4( 1,  1, -1, 0),
        glm::i8vec4( 1,  1,  1, 0),
        glm::i8vec4(-1,  1,  1, 0),
        glm::i8vec4(-1,  1, -1, 0),

        // front
        glm::i8vec4( 0,  0, -1, 0),
        glm::i8vec4(-1, -1, -1, 0),
        glm::i8vec4( 1, -1, -1, 0),
        glm::i8vec4( 1,  1, -1, 0),
        glm::i8vec4(-1,  1, -1, 0),
        glm::i8vec4(-1, -1, -1, 0),

        // back
        glm::i8vec4( 0,  0,  1, 0),
        glm::i8vec4(-1, -1,  1, 0),
        glm::i8vec4(-1,  1,  1, 0),
        glm::i8vec4( 1,  1,  1, 0),
        glm::i8vec4( 1, -1,  1, 0),
        glm::i8vec4(-1, -1,  1, 0),
    };

    static glm::i8vec4 const normals[] =
    {
        // left
        glm::i8vec4(-1,  0,  0, 0),
        glm::i8vec4(-1,  0,  0, 0),
        glm::i8vec4(-1,  0,  0, 0),
        glm::i8vec4(-1,  0,  0, 0),
        glm::i8vec4(-1,  0,  0, 0),
        glm::i8vec4(-1,  0,  0, 0),

        // right
        glm::i8vec4( 1,  0,  0, 0),
        glm::i8vec4( 1,  0,  0, 0),
        glm::i8vec4( 1,  0,  0, 0),
        glm::i8vec4( 1,  0,  0, 0),
        glm::i8vec4( 1,  0,  0, 0),
        glm::i8vec4( 1,  0,  0, 0),

        // bottom
        glm::i8vec4( 0, -1,  0, 0),
        glm::i8vec4( 0, -1,  0, 0),
        glm::i8vec4( 0, -1,  0, 0),
        glm::i8vec4( 0, -1,  0, 0),
        glm::i8vec4( 0, -1,  0, 0),
        glm::i8vec4( 0, -1,  0, 0),

        // top
        glm::i8vec4( 0,  1,  0, 0),
        glm::i8vec4( 0,  1,  0, 0),
        glm::i8vec4( 0,  1,  0, 0),
        glm::i8vec4( 0,  1,  0, 0),
        glm::i8vec4( 0,  1,  0, 0),
        glm::i8vec4( 0,  1,  0, 0),

        // front
        glm::i8vec4( 0,  0, -1, 0),
        glm::i8vec4( 0,  0, -1, 0),
        glm::i8vec4( 0,  0, -1, 0),
        glm::i8vec4( 0,  0, -1, 0),
        glm::i8vec4( 0,  0, -1, 0),
        glm::i8vec4( 0,  0, -1, 0),

        // back
        glm::i8vec4( 0,  0,  1, 0),
        glm::i8vec4( 0,  0,  1, 0),
        glm::i8vec4( 0,  0,  1, 0),
        glm::i8vec4( 0,  0,  1, 0),
        glm::i8vec4( 0,  0,  1, 0),
        glm::i8vec4( 0,  0,  1, 0),
    };

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbo_vertex);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_vertex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribIPointer(BLOCKGAME_ATTRIB_VERTEX, 4, GL_BYTE, 0, NULL);
    glEnableVertexAttribArray(BLOCKGAME_ATTRIB_VERTEX);

    glGenBuffers(1, &this->vbo_normal);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_normal);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
    glVertexAttribIPointer(BLOCKGAME_ATTRIB_NORMAL, 4, GL_BYTE, 0, NULL);
    glEnableVertexAttribArray(BLOCKGAME_ATTRIB_NORMAL);

    return true;
}

bool BlockGame::HandleSDL(SDL_Event *e)
{
    switch(e->type)
    {
        case SDL_QUIT:
            return false;
            break;
    }

    return true;
}

bool BlockGame::Draw(void)
{
    static float r = 0.0f;
    r += 0.5f;

    glClearColor(0.6f, 0.65f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 matIdentity(1.0f);
    glm::mat4 matProjection = glm::perspective(35.0f, 1280.0f / 720.0f, 1.0f, 100.0f);
    glm::mat4 matModelView = glm::translate(matIdentity, glm::vec3(0.0f, 0.0f, -4.0f));
    matModelView = glm::rotate(matModelView, 30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    matModelView = glm::rotate(matModelView, r, glm::vec3(0.0f, 1.0f, 0.0f));

    GLint u_matProjection = glGetUniformLocation(this->program_id, "u_matProjection");
    GLint u_matModelView = glGetUniformLocation(this->program_id, "u_matModelView");
    glUniformMatrix4fv(u_matProjection, 1, GL_FALSE, glm::value_ptr(matProjection));
    glUniformMatrix4fv(u_matModelView, 1, GL_FALSE, glm::value_ptr(matModelView));

    //glBindVertexArray(this->vao);
    GLint firsts[] = {0, 6, 12, 18, 24, 30};
    GLint counts[] = {6, 6, 6, 6, 6, 6};
    glMultiDrawArrays(GL_TRIANGLE_FAN, firsts, counts, sizeof(firsts) / sizeof(GLint));

    SDL_GL_SwapWindow(this->wnd);

    return true;
}

bool BlockGame::Destroy(void)
{
    if(!this->DestroySDL()) return false;
    return true;
}

int BlockGame::Run(int argc, const char **argv)
{
    if(!this->Init()) return 1;

    SDL_Event e;
    for(this->running=true; this->running;)
    {
        if(!this->Draw()) this->running = false;

        while(SDL_PollEvent(&e))
        {
            if(!this->HandleSDL(&e))
            {
                this->running = false;
                break;
            }
        }
    }

    if(!this->Destroy()) return 1;

    return 0;
}
