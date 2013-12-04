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

#include "Game.h"

#define GAME_DOMAIN "Game::PrintShaderError"
void Game::PrintShaderError(GLint shader)
{
    GLint max_len = 0;
    GLint log_len = 0;
    ASSERT_GL(unsigned char is_shader = glIsShader(shader))

    if(is_shader)
    {
        ASSERT_GL(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_len))
    }
    else
    {
        ASSERT_GL(glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &max_len))
    }

    if(max_len <= 0) return;

    GLchar *message = (GLchar *)malloc(sizeof(GLchar) * max_len);

    if(is_shader)
    {
        ASSERT_GL(glGetShaderInfoLog(shader, max_len, &log_len, message))
    }
    else
    {
        ASSERT_GL(glGetProgramInfoLog(shader, max_len, &log_len, message))
    }

    fprintf(stderr, "%s\n", message);
    free(message);
}
#undef GAME_DOMAIN

#define GAME_DOMAIN "Game::InitSDL"
bool Game::InitSDL(void)
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
    ASSERT_GL(fprintf(stderr, "[DUMMY] OpenGL version: %s\n", glGetString(GL_VERSION)))
    ASSERT_GL(fprintf(stderr, "[DUMMY] GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION)))

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

    // set initial aspect ratio
    this->aspect = this->width / this->height;

    // create real window and context

    this->wnd = SDL_CreateWindow("Game",
                                 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 this->width, this->height,
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
    ASSERT_GL(fprintf(stderr, "OpenGL version: %s\n", glGetString(GL_VERSION)))
    ASSERT_GL(fprintf(stderr, "GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION)))

    // enable vsync
    SDL_GL_SetSwapInterval(1);

    return true;
}
#undef GAME_DOMAIN

#define GAME_DOMAIN "Game::InitGLEW"
bool Game::InitGLEW(void)
{
#if defined(_WIN32) || defined(__linux__)
    glewExperimental = true;

    ASSERT_GL(GLenum err = glewInit())
    if(err != GLEW_OK)
    {
        fprintf(stderr, "glewInit: error\n");
        return true;
    }
#endif

    return true;
}
#undef GAME_DOMAIN

#define GAME_DOMAIN "Game::InitShaders"
bool Game::InitShaders(const char *v_path, const char *f_path)
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

    ASSERT_GL(GLuint v_id = glCreateShader(GL_VERTEX_SHADER))
    ASSERT_GL(GLuint f_id = glCreateShader(GL_FRAGMENT_SHADER))

    ASSERT_GL(glShaderSource(v_id, 1, &v_src, (GLint *)&v_len))
    ASSERT_GL(glShaderSource(f_id, 1, &f_src, (GLint *)&f_len))

    GLint compile_status;

    ASSERT_GL(glCompileShader(v_id))
    ASSERT_GL(glGetShaderiv(v_id, GL_COMPILE_STATUS, &compile_status))
    if(!compile_status)
    {
        fprintf(stderr, "glCompileShader(v_id): error\n");
        this->PrintShaderError(v_id);
        return false;
    }

    ASSERT_GL(glCompileShader(f_id))
    ASSERT_GL(glGetShaderiv(f_id, GL_COMPILE_STATUS, &compile_status))
    if(!compile_status)
    {
        fprintf(stderr, "glCompileShader(f_id): error\n");
        this->PrintShaderError(f_id);
        return false;
    }

    ASSERT_GL(this->program_id = glCreateProgram())
    ASSERT_GL(glAttachShader(this->program_id, v_id))
    ASSERT_GL(glAttachShader(this->program_id, f_id))

    ASSERT_GL(glBindAttribLocation(this->program_id, GAME_ATTRIB_VERTEX, "a_vVertex"))
    ASSERT_GL(glBindAttribLocation(this->program_id, GAME_ATTRIB_NORMAL, "a_vNormal"))
    ASSERT_GL(glBindAttribLocation(this->program_id, GAME_ATTRIB_TEXCOORD, "a_vTexCoord"))

    ASSERT_GL(glLinkProgram(this->program_id))
    ASSERT_GL(glUseProgram(this->program_id))

    return true;
}
#undef GAME_DOMAIN

bool Game::DestroySDL(void)
{
    SDL_DestroyWindow(this->wnd);
    SDL_Quit();
    return true;
}

#define GAME_DOMAIN "Game::Init"
bool Game::Init(void)
{
    // init random
    srand((unsigned int)time(NULL));

    this->width = 1280;
    this->height = 720;

    if(!this->InitSDL()) return false;
    if(!this->InitGLEW()) return false;
    if(!this->InitShaders("shader.vsh", "shader.fsh")) return false;

    LightingManager::Init(program_id);

    //LightingManager::light_types[0].vDiffuse = glm::vec4(1, 0, 0, 1);
    LightingManager::light_types[0].vDiffuse = glm::vec4(1, 0.8f, 0, 1);
    LightingManager::light_types[1].vDiffuse = glm::vec4(0, 1, 0, 1);
    LightingManager::light_types[2].vDiffuse = glm::vec4(0, 0, 1, 1);

    LightingManager::light_types[0].vSpecular = glm::vec4(1, 0.5f, 0.5f, 1);
    LightingManager::light_types[1].vSpecular = glm::vec4(0.5f, 1, 0.5f, 1);
    LightingManager::light_types[2].vSpecular = glm::vec4(0.5f, 0.5f, 1, 1);

    LightingManager::light_types[0].fAttenuationQuadratic = 0.05f;
    LightingManager::light_types[1].fAttenuationQuadratic = 0.05f;
    LightingManager::light_types[2].fAttenuationQuadratic = 0.05f;

    LightingManager::MakeLight(0, true, 0, glm::vec4( 1.5,  1.5, 1.5, 1));
    LightingManager::MakeLight(1, true, 1, glm::vec4(-1.5,  1.5, 1.5, 1));
    LightingManager::MakeLight(2, true, 2, glm::vec4(-1.5, -1.5, 1.5, 1));
    //LightingManager::MakeLight(0, true, 0, glm::vec4(0, 1.5,  1, 1));
    //LightingManager::MakeLight(1, true, 1, glm::vec4(0, 1.5, -1, 1));

    LightingManager::materials[0].fShininess = 64;

    LightingManager::materials[1].fShininess = 16;
    LightingManager::materials[1].vDiffuse = glm::vec4(1, 0.9f, 0.7f, 1);

    LightingManager::UploadAll(this->program_id);

    cube_left.Init(program_id);
    cube_left.position = glm::vec3(-2.0f, 0, 0);

    cube_right.Init(program_id);
    cube_right.position = glm::vec3(2.0f, 0, 0);
    cube_right.material_id = 1;

    particles.Init(program_id);

    ASSERT_GL(glEnable(GL_DEPTH_TEST))
    ASSERT_GL(glEnable(GL_CULL_FACE))
    ASSERT_GL(glCullFace(GL_BACK))
    ASSERT_GL(glEnable(GL_BLEND))
    ASSERT_GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))
    ASSERT_GL(glEnable(GL_VERTEX_PROGRAM_POINT_SIZE))

    this->matIdentity = glm::mat4(1.0f);
    this->camera = glm::translate(this->matIdentity, glm::vec3(0.0f, 0.0f, -5.0f));

    // max anisotropy
    GLfloat aniso;
    ASSERT_GL(glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso))

    // load textures
    this->tex = TextureManager::LoadBMP("stone.bmp", GL_TEXTURE0, aniso);
    this->nmap = TextureManager::LoadBMP("four_NM_height.bmp", GL_TEXTURE1, aniso, false, true);
    this->glossmap = TextureManager::LoadBMP("stone_gloss.bmp", GL_TEXTURE2, aniso);
    TextureManager::LoadBMP("stone_normal.bmp", GL_TEXTURE3, aniso);

    // bind texture units to shader samplers
    ASSERT_GL(glUniform1i(glGetUniformLocation(this->program_id, "u_sDiffuse"), 0))
    ASSERT_GL(glUniform1i(glGetUniformLocation(this->program_id, "u_sNormalHeight"), 1))
    ASSERT_GL(glUniform1i(glGetUniformLocation(this->program_id, "u_sSpecular"), 2))
    ASSERT_GL(glUniform1i(glGetUniformLocation(this->program_id, "u_sNormalHeight2"), 3))

    // upload projection matrix
    glm::mat4 matProjection = glm::perspective(35.0f, this->aspect, 0.01f, 100.0f);
    ASSERT_GL(GLint u_matProjection = glGetUniformLocation(this->program_id, "u_matProjection"))
    ASSERT_GL(glUniformMatrix4fv(u_matProjection, 1, GL_FALSE, glm::value_ptr(matProjection)))

    // exposure
    LightingManager::SetExposure(program_id, 1);

    return true;
}
#undef GAME_DOMAIN

#define GAME_DOMAIN "Game::HandleSDL"
bool Game::HandleSDL(SDL_Event *e)
{
    switch(e->type)
    {
        case SDL_QUIT:
            return false;
        case SDL_KEYDOWN:
            // remove key if already in pressed_keys
            for(std::vector<SDL_Keycode>::iterator i=this->pressed_keys.begin();
                i!=this->pressed_keys.end();
                ++i)
            {
                if(*i == e->key.keysym.sym)
                {
                    this->pressed_keys.erase(i);
                    break;
                }
            }

            this->pressed_keys.push_back(e->key.keysym.sym);
            break;
        case SDL_KEYUP:
            // remove key if in pressed_keys
            for(std::vector<SDL_Keycode>::iterator i=this->pressed_keys.begin();
                i!=this->pressed_keys.end(); ++i)
            {
                if(*i == e->key.keysym.sym)
                {
                    this->pressed_keys.erase(i);
                    break;
                }
            }
            break;
        case SDL_WINDOWEVENT:
            switch(e->window.event)
            {
                case SDL_WINDOWEVENT_RESIZED:
                    this->width = e->window.data1;
                    this->height = e->window.data2;
                    this->aspect = this->width / this->height;
                    ASSERT_GL(glViewport(0, 0, this->width, this->height))

                    // upload new projection matrix
                    glm::mat4 matProjection = glm::perspective(35.0f, this->aspect, 0.01f, 100.0f);
                    ASSERT_GL(GLint u_matProjection = glGetUniformLocation(program_id, "u_matProjection"))
                    ASSERT_GL(glUniformMatrix4fv(u_matProjection, 1, GL_FALSE, glm::value_ptr(matProjection)))

                    break;
            }
            break;
    }

    return true;
}
#undef GAME_DOMAIN

bool Game::Update(float seconds)
{
    // particles
    particles.Update(seconds * 1000);

    const float movement_speed = 1.5f;
    const float rotation_speed = 70.0f;

    for(std::vector<SDL_Keycode>::iterator i=this->pressed_keys.begin();
        i!=this->pressed_keys.end(); ++i)
    {
        switch(*i)
        {
            case SDLK_ESCAPE:
                return false;
            case SDLK_w:
                this->camera = glm::translate(this->matIdentity, glm::vec3(0, 0, movement_speed * seconds)) * this->camera;
                break;
            case SDLK_s:
                this->camera = glm::translate(this->matIdentity, glm::vec3(0, 0, -movement_speed * seconds)) * this->camera;
                break;
            case SDLK_a:
                this->camera = glm::translate(this->matIdentity, glm::vec3(movement_speed * seconds, 0, 0)) * this->camera;
                break;
            case SDLK_d:
                this->camera = glm::translate(this->matIdentity, glm::vec3(-movement_speed * seconds, 0, 0)) * this->camera;
                break;
            case SDLK_SPACE:
                this->camera = glm::translate(this->matIdentity, glm::vec3(0, -movement_speed * seconds, 0)) * this->camera;
                break;
            case SDLK_c:
                this->camera = glm::translate(this->matIdentity, glm::vec3(0, movement_speed * seconds, 0)) * this->camera;
                break;
            case SDLK_UP:
                this->camera = glm::rotate(this->matIdentity, -rotation_speed * seconds, glm::vec3(1, 0, 0)) * this->camera;
                break;
            case SDLK_DOWN:
                this->camera = glm::rotate(this->matIdentity, rotation_speed * seconds, glm::vec3(1, 0, 0)) * this->camera;
                break;
            case SDLK_LEFT:
                this->camera = glm::rotate(this->matIdentity, -rotation_speed * seconds, glm::vec3(0, 1, 0)) * this->camera;
                break;
            case SDLK_RIGHT:
                this->camera = glm::rotate(this->matIdentity, rotation_speed * seconds, glm::vec3(0, 1, 0)) * this->camera;
                break;
            case SDLK_q:
                this->camera = glm::rotate(this->matIdentity, -rotation_speed * seconds, glm::vec3(0, 0, 1)) * this->camera;
                break;
            case SDLK_e:
                this->camera = glm::rotate(this->matIdentity, rotation_speed * seconds, glm::vec3(0, 0, 1)) * this->camera;
                break;
        }
    }

    return true;
}

#define GAME_DOMAIN "Game::Draw"
bool Game::Draw(void)
{
    //ASSERT_GL(glClearColor(0.6f, 0.65f, 0.9f, 1.0f))
    ASSERT_GL(glClearColor(0.02, 0.05, 0.1, 1))
    ASSERT_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))

    glm::mat4 matCamera = this->camera;

    ASSERT_GL(GLint u_matCamera = glGetUniformLocation(this->program_id, "u_matCamera"))
    ASSERT_GL(GLint u_matModelView = glGetUniformLocation(this->program_id, "u_matModelView"))

    ASSERT_GL(glUniformMatrix4fv(u_matCamera, 1, GL_FALSE, glm::value_ptr(matCamera)))

    cube_left.Draw(program_id, u_matModelView, matCamera);
    cube_right.Draw(program_id, u_matModelView, matCamera);
    particles.Draw(program_id, u_matModelView, matCamera);

    SDL_GL_SwapWindow(this->wnd);

    return true;
}
#undef GAME_DOMAIN

bool Game::Destroy(void)
{
    if(!this->DestroySDL()) return false;
    return true;
}

int Game::Run(int argc, const char **argv)
{
    if(!this->Init()) return 1;

    Uint32 current_time = SDL_GetTicks();
    Uint32 previous_time;
    float accumulator = 0.0f;
    const float timestep = 1.0f / 120.0f;

    SDL_Event e;
    for(this->running=true; this->running;)
    {
        previous_time = current_time;
        current_time = SDL_GetTicks();
        accumulator += (current_time - previous_time) / 1000.0f;

        for(;accumulator >= timestep; accumulator -= timestep)
        {
            if(!this->Update(timestep))
            {
                this->running = false;
                break;
            }
        }

        if(this->running)
        {
            if(!this->Draw()) this->running = false;
        }

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
