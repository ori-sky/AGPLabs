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
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

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
bool Game::InitShaders(const char *v_path, const char *f_path, GLuint *program)
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
        fprintf(stderr, "glCompileShader(v_id): error in `%s`\n", f_path);
        this->PrintShaderError(v_id);
        return false;
    }

    ASSERT_GL(glCompileShader(f_id))
    ASSERT_GL(glGetShaderiv(f_id, GL_COMPILE_STATUS, &compile_status))
    if(!compile_status)
    {
        fprintf(stderr, "glCompileShader(f_id): error `%s`\n", f_path);
        this->PrintShaderError(f_id);
        return false;
    }

    ASSERT_GL(*program = glCreateProgram())
    ASSERT_GL(glAttachShader(*program, v_id))
    ASSERT_GL(glAttachShader(*program, f_id))

    //ASSERT_GL(glBindAttribLocation(*program, GAME_ATTRIB_VERTEX, "a_vVertex"))
    //ASSERT_GL(glBindAttribLocation(*program, GAME_ATTRIB_NORMAL, "a_vNormal"))
    //ASSERT_GL(glBindAttribLocation(*program, GAME_ATTRIB_TEXCOORD, "a_vTexCoord"))

    ASSERT_GL(glLinkProgram(*program))
    ASSERT_GL(glUseProgram(*program))

	// fixes viewport starting at the wrong size
	ASSERT_GL(glViewport(0, 0, width, height))

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

    // initial states
    b_hdr = false;
    b_bloom = false;
    b_motionblur = false;
    b_particles_create = true;
    b_particles_update = true;

    this->width = 1280;
    this->height = 720;

    if(!this->InitSDL()) return false;
    if(!this->InitGLEW()) return false;
    if(!this->InitShaders("shader.vsh", "shader.fsh", &program_id)) return false;
    ASSERT_GL(loc_u_bHDR = glGetUniformLocation(program_id, "u_bHDR"))
    ASSERT_GL(glProgramUniform1i(program_id, loc_u_bHDR, b_hdr))

    LightingManager::Init(program_id);

    //LightingManager::light_types[0].vDiffuse = glm::vec4(1, 0, 0, 1);
    LightingManager::light_types[0].vDiffuse = glm::vec4(1, 0.8f, 0, 1);
    LightingManager::light_types[1].vDiffuse = glm::vec4(0, 1, 0, 1);
    LightingManager::light_types[2].vDiffuse = glm::vec4(0, 0, 1, 1);
    LightingManager::light_types[3].vDiffuse = glm::vec4(1, 1, 1, 1);

    LightingManager::light_types[0].vSpecular = glm::vec4(1, 0.5f, 0.5f, 1);
    LightingManager::light_types[1].vSpecular = glm::vec4(0.5f, 1, 0.5f, 1);
    LightingManager::light_types[2].vSpecular = glm::vec4(0.5f, 0.5f, 1, 1);
    LightingManager::light_types[3].vSpecular = glm::vec4(1, 1, 1, 1);

    LightingManager::light_types[0].fAttenuationQuadratic = 0.05f;
    LightingManager::light_types[1].fAttenuationQuadratic = 0.05f;
    LightingManager::light_types[2].fAttenuationQuadratic = 0.05f;
    LightingManager::light_types[3].fAttenuationQuadratic = 0.6f;

    LightingManager::MakeLight(0, true, 0, glm::vec4( 1.5,  1.5, 1.5, 1));
    LightingManager::MakeLight(1, true, 1, glm::vec4(-1.5,  1.5, 1.5, 1));
    LightingManager::MakeLight(2, true, 2, glm::vec4(-1.5,  1.5, 1.5, 1));
    LightingManager::MakeLight(3, true, 3, glm::vec4(-0.4, 0, 0, 1));
    //LightingManager::MakeLight(0, true, 0, glm::vec4(0, 1.5,  1, 1));
    //LightingManager::MakeLight(1, true, 1, glm::vec4(0, 1.5, -1, 1));

    // particles
    LightingManager::MakeLightType(5, glm::vec4(0), glm::vec4(0.7f, 0.9f, 1, 1), glm::vec4(0.8f, 0.9f, 1, 1), 0.2, 0.0f, 15.0f);
    for(unsigned int i=5; i<NUM_LIGHTS; ++i)
    {
        LightingManager::MakeLight(i, false, 5, glm::vec4(0, 0, 0, 1));
    }

    LightingManager::materials[0].fShininess = 128;

    LightingManager::materials[1].fShininess = 16;
    LightingManager::materials[1].vDiffuse = glm::vec4(1, 0.9f, 0.7f, 1);

    LightingManager::UploadAll(this->program_id);

    cube_left.Init(program_id);
    cube_left.position = glm::vec3(-2.0f, 0, 0);

    cube_right.Init(program_id);
    cube_right.position = glm::vec3(2.0f, 0, 0);
    cube_right.material_id = 1;

    particles.Init(program_id);
    particles.b_create = b_particles_create;

    ASSERT_GL(glEnable(GL_DEPTH_TEST))
    ASSERT_GL(glEnable(GL_CULL_FACE))
    ASSERT_GL(glCullFace(GL_BACK))
    ASSERT_GL(glEnable(GL_BLEND))
    ASSERT_GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))
    ASSERT_GL(glEnable(GL_VERTEX_PROGRAM_POINT_SIZE))
    ASSERT_GL(glEnable(GL_MULTISAMPLE))

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

    // bloom

    ASSERT_GL(glActiveTexture(GL_TEXTURE5))
    ASSERT_GL(glGenTextures(1, &bloom_tex_fbo))
    ASSERT_GL(glBindTexture(GL_TEXTURE_2D, bloom_tex_fbo))
    ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR))
    ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR))
    ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE))
    ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE))
    ASSERT_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL))
    ASSERT_GL(glBindTexture(GL_TEXTURE_2D, 0))

    ASSERT_GL(glGenRenderbuffers(1, &bloom_rbo_depth))
    ASSERT_GL(glBindRenderbuffer(GL_RENDERBUFFER, bloom_rbo_depth))
    ASSERT_GL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height))
    ASSERT_GL(glBindRenderbuffer(GL_RENDERBUFFER, 0))

    ASSERT_GL(glGenFramebuffers(1, &bloom_fbo))
    ASSERT_GL(glBindFramebuffer(GL_FRAMEBUFFER, bloom_fbo))
    ASSERT_GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bloom_tex_fbo, 0))
    ASSERT_GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, bloom_rbo_depth))

    GLenum status;
    if((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
    {
        fprintf(stderr, "framebuffer error 0x%x\n", status);
    }

    ASSERT_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0))

    GLfloat vertices_fbo[] =
    {
        -1, -1,
         1, -1,
        -1,  1,
         1,  1,
    };

    ASSERT_GL(glGenBuffers(1, &bloom_vbo_fbo_vertices))
    ASSERT_GL(glBindBuffer(GL_ARRAY_BUFFER, bloom_vbo_fbo_vertices))
    ASSERT_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_fbo), vertices_fbo, GL_STATIC_DRAW))
    ASSERT_GL(glBindBuffer(GL_ARRAY_BUFFER, 0))

    // bloom program
    if(!this->InitShaders("postproc_bloom.vsh", "postproc_bloom.fsh", &program_bloom)) return false;
    ASSERT_GL(bloom_loc_fbo_a_vCoord = glGetAttribLocation(program_bloom, "a_vCoord"))
    ASSERT_GL(bloom_loc_fbo_u_sFBO = glGetUniformLocation(program_bloom, "u_sFBO"))
    ASSERT_GL(bloom_loc_fbo_u_bEnabled = glGetUniformLocation(program_bloom, "u_bEnabled"))
    ASSERT_GL(glProgramUniform1i(program_bloom, bloom_loc_fbo_u_bEnabled, b_bloom))

    // motionblur

    ASSERT_GL(glActiveTexture(GL_TEXTURE6))
    ASSERT_GL(glGenTextures(1, &motionblur_tex_fbo))
    ASSERT_GL(glBindTexture(GL_TEXTURE_2D, motionblur_tex_fbo))
    ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR))
    ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR))
    ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE))
    ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE))
    ASSERT_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL))
    ASSERT_GL(glBindTexture(GL_TEXTURE_2D, 0))

    ASSERT_GL(glGenRenderbuffers(1, &motionblur_rbo_depth))
    ASSERT_GL(glBindRenderbuffer(GL_RENDERBUFFER, motionblur_rbo_depth))
    ASSERT_GL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height))
    ASSERT_GL(glBindRenderbuffer(GL_RENDERBUFFER, 0))

    ASSERT_GL(glGenFramebuffers(1, &motionblur_fbo))
    ASSERT_GL(glBindFramebuffer(GL_FRAMEBUFFER, motionblur_fbo))
    ASSERT_GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, motionblur_tex_fbo, 0))
    ASSERT_GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, motionblur_rbo_depth))

    if((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
    {
        fprintf(stderr, "framebuffer error 0x%x\n", status);
    }

    ASSERT_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0))

    ASSERT_GL(glGenBuffers(1, &motionblur_vbo_fbo_vertices))
    ASSERT_GL(glBindBuffer(GL_ARRAY_BUFFER, motionblur_vbo_fbo_vertices))
    ASSERT_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_fbo), vertices_fbo, GL_STATIC_DRAW))
    ASSERT_GL(glBindBuffer(GL_ARRAY_BUFFER, 0))

    // motionblur program
    if(!this->InitShaders("postproc_motionblur.vsh", "postproc_motionblur.fsh", &program_motionblur)) return false;
    ASSERT_GL(motionblur_loc_fbo_a_vCoord = glGetAttribLocation(program_motionblur, "a_vCoord"))
    ASSERT_GL(motionblur_loc_fbo_u_sFBO = glGetUniformLocation(program_motionblur, "u_sFBO"))
    ASSERT_GL(motionblur_loc_fbo_u_bEnabled = glGetUniformLocation(program_motionblur, "u_bEnabled"))
    ASSERT_GL(motionblur_loc_fbo_u_vVelocity = glGetUniformLocation(program_motionblur, "u_vVelocity"))
    ASSERT_GL(glProgramUniform1i(program_motionblur, motionblur_loc_fbo_u_bEnabled, b_motionblur))

    // shadow mapping

    /*ASSERT_GL(glGenFramebuffers(1, &fb_shadow))
    ASSERT_GL(glBindFramebuffer(GL_FRAMEBUFFER, fb_shadow))

    GLuint depth;
    ASSERT_GL(glGenTextures(1, &depth))
    ASSERT_GL(glBindTexture(GL_TEXTURE_2D, depth))
    ASSERT_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0))
    ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST))
    ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST))
    ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE))
    ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE))

    ASSERT_GL(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth, 0))
    ASSERT_GL(glDrawBuffer(GL_NONE))

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        fprintf(stderr, "framebuffer not complete\n");
        return false;
    }*/

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

            switch(e->key.keysym.sym)
            {
                case SDLK_1: // toggle HDR
                    b_hdr = !b_hdr;
                    ASSERT_GL(glProgramUniform1i(program_id, loc_u_bHDR, b_hdr))
                    break;
                case SDLK_2: // toggle bloom
                    b_bloom = !b_bloom;
                    ASSERT_GL(glProgramUniform1i(program_bloom, bloom_loc_fbo_u_bEnabled, b_bloom))
                    break;
                case SDLK_3: // toggle motion blur
                    b_motionblur = !b_motionblur;
                    ASSERT_GL(glProgramUniform1i(program_motionblur, motionblur_loc_fbo_u_bEnabled, b_motionblur))
                    break;
                case SDLK_4: // toggle particles create
                    b_particles_create = !b_particles_create;
                    particles.b_create = b_particles_create;
                    break;
                case SDLK_5: // toggle particles update
                    b_particles_update = !b_particles_update;
                    break;
            }

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
                    // fixes weird SDL bug on Windows
                    if(e->window.data2 < 20) break;

                    width = e->window.data1;
                    height = e->window.data2;
                    aspect = width / height;

                    // rescale bloom stuff
                    ASSERT_GL(glBindTexture(GL_TEXTURE_2D, bloom_tex_fbo))
                    ASSERT_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL))
                    ASSERT_GL(glBindTexture(GL_TEXTURE_2D, 0))
                    ASSERT_GL(glBindRenderbuffer(GL_RENDERBUFFER, bloom_rbo_depth))
                    ASSERT_GL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height))
                    ASSERT_GL(glBindRenderbuffer(GL_RENDERBUFFER, 0))

                    // rescale motionblur stuff
                    ASSERT_GL(glBindTexture(GL_TEXTURE_2D, motionblur_tex_fbo))
                    ASSERT_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL))
                    ASSERT_GL(glBindTexture(GL_TEXTURE_2D, 0))
                    ASSERT_GL(glBindRenderbuffer(GL_RENDERBUFFER, motionblur_rbo_depth))
                    ASSERT_GL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height))
                    ASSERT_GL(glBindRenderbuffer(GL_RENDERBUFFER, 0))

                    ASSERT_GL(glViewport(0, 0, width, height))

                    // upload new projection matrix
                    glm::mat4 matProjection = glm::perspective(35.0f, aspect, 0.01f, 100.0f);
                    ASSERT_GL(GLint u_matProjection = glGetUniformLocation(program_id, "u_matProjection"))
                    ASSERT_GL(glUniformMatrix4fv(u_matProjection, 1, GL_FALSE, glm::value_ptr(matProjection)))

                    break;
            }
            break;
    }

    return true;
}
#undef GAME_DOMAIN

#define GAME_DOMAIN "Game::Update"
bool Game::Update(float seconds)
{
    static float f = 0;
    f += seconds;

    // moving light
    //LightingManager::lights[2].vPosition.x = -2 * cos(2 * f);
    //LightingManager::lights[2].vPosition.z = -2 * sin(2 * f);

    LightingManager::UploadLights(program_id);

    // particles
    if(b_particles_update) particles.Update(seconds * 1000);

    const float acceleration = 15;
    const float rotation_acceleration = 400;
    const float rotation_speed = 90.0f;

    for(std::vector<SDL_Keycode>::iterator i=this->pressed_keys.begin(); i!=this->pressed_keys.end(); ++i)
    {
        switch(*i)
        {
            case SDLK_ESCAPE:
                return false;
            case SDLK_w:
                velocity.z += acceleration * seconds;
                break;
            case SDLK_s:
                velocity.z -= acceleration * seconds;
                break;
            case SDLK_a:
                velocity.x += acceleration * seconds;
                break;
            case SDLK_d:
                velocity.x -= acceleration * seconds;
                break;
            case SDLK_SPACE:
                velocity.y -= acceleration * seconds;
                break;
            case SDLK_c:
                velocity.y += acceleration * seconds;
                break;
            case SDLK_UP:
                rotation_velocity.x -= rotation_acceleration * seconds;
                //this->camera = glm::rotate(this->matIdentity, -rotation_speed * seconds, glm::vec3(1, 0, 0)) * this->camera;
                break;
            case SDLK_DOWN:
                rotation_velocity.x += rotation_acceleration * seconds;
                //this->camera = glm::rotate(this->matIdentity, rotation_speed * seconds, glm::vec3(1, 0, 0)) * this->camera;
                break;
            case SDLK_LEFT:
                rotation_velocity.y -= rotation_acceleration * seconds;
                //this->camera = glm::rotate(this->matIdentity, -rotation_speed * seconds, glm::vec3(0, 1, 0)) * this->camera;
                break;
            case SDLK_RIGHT:
                rotation_velocity.y += rotation_acceleration * seconds;
                //this->camera = glm::rotate(this->matIdentity, rotation_speed * seconds, glm::vec3(0, 1, 0)) * this->camera;
                break;
            case SDLK_q:
                rotation_velocity.z -= rotation_acceleration * seconds;
                //this->camera = glm::rotate(this->matIdentity, -rotation_speed * seconds, glm::vec3(0, 0, 1)) * this->camera;
                break;
            case SDLK_e:
                rotation_velocity.z += rotation_acceleration * seconds;
                //this->camera = glm::rotate(this->matIdentity, rotation_speed * seconds, glm::vec3(0, 0, 1)) * this->camera;
                break;
        }
    }

    camera = glm::translate(matIdentity, velocity * seconds) * camera;
    velocity *= fmax(0, 1 - (3.5f * seconds));

    camera = glm::rotate(matIdentity, rotation_velocity.x * seconds, glm::vec3(1, 0, 0)) * camera;
    camera = glm::rotate(matIdentity, rotation_velocity.y * seconds, glm::vec3(0, 1, 0)) * camera;
    camera = glm::rotate(matIdentity, rotation_velocity.z * seconds, glm::vec3(0, 0, 1)) * camera;
    rotation_velocity *= fmax(0, 1 - (4.0f * seconds));

    ASSERT_GL(glProgramUniform3fv(program_motionblur, motionblur_loc_fbo_u_vVelocity, 1, glm::value_ptr(velocity)))

    return true;
}
#undef GAME_DOMAIN

#define GAME_DOMAIN "Game::Draw"
bool Game::Draw(void)
{
    // SCENE

    // bind bloom framebuffer and use normal program
    ASSERT_GL(glBindFramebuffer(GL_FRAMEBUFFER, bloom_fbo))
    ASSERT_GL(glUseProgram(program_id))

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

    // BLOOM

    // bind motionblur framebuffer and use bloom program
    ASSERT_GL(glBindFramebuffer(GL_FRAMEBUFFER, motionblur_fbo))
    ASSERT_GL(glUseProgram(program_bloom))

    ASSERT_GL(glClearColor(0, 0, 0, 1))
    ASSERT_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))

    // draw framebuffer as texture
    ASSERT_GL(glActiveTexture(GL_TEXTURE5))
    ASSERT_GL(glBindTexture(GL_TEXTURE_2D, bloom_tex_fbo))
    ASSERT_GL(glUniform1i(bloom_loc_fbo_u_sFBO, 5))

    ASSERT_GL(glEnableVertexAttribArray(bloom_loc_fbo_a_vCoord))
    ASSERT_GL(glBindBuffer(GL_ARRAY_BUFFER, bloom_vbo_fbo_vertices))
    ASSERT_GL(glVertexAttribPointer(bloom_loc_fbo_a_vCoord, 2, GL_FLOAT, GL_FALSE, 0, 0))
    ASSERT_GL(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4))
    ASSERT_GL(glDisableVertexAttribArray(bloom_loc_fbo_a_vCoord))

    // MOTIONBLUR

    // unbind framebuffer and use motionblur program
    ASSERT_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0))
    ASSERT_GL(glUseProgram(program_motionblur))

    ASSERT_GL(glClearColor(0, 0, 0, 1))
    ASSERT_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))

    // draw framebuffer as texture
    ASSERT_GL(glActiveTexture(GL_TEXTURE6))
    ASSERT_GL(glBindTexture(GL_TEXTURE_2D, motionblur_tex_fbo))
    ASSERT_GL(glUniform1i(motionblur_loc_fbo_u_sFBO, 6))

    ASSERT_GL(glEnableVertexAttribArray(motionblur_loc_fbo_a_vCoord))
    ASSERT_GL(glBindBuffer(GL_ARRAY_BUFFER, motionblur_vbo_fbo_vertices))
    ASSERT_GL(glVertexAttribPointer(motionblur_loc_fbo_a_vCoord, 2, GL_FLOAT, GL_FALSE, 0, 0))
    ASSERT_GL(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4))
    ASSERT_GL(glDisableVertexAttribArray(motionblur_loc_fbo_a_vCoord))

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
