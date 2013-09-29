//
//  main.cpp
//  AGPLab1
//
//  Created by Shockk on 27/09/2013.
//  Copyright (c) 2013 Shockk. All rights reserved.
//

#include <stdio.h>
#include <string>

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

#define ATTRIB_VERTEX   0
#define ATTRIB_COLOR    1
#define ATTRIB_NORMAL   2
#define ATTRIB_TEXCOORD 3
#define ATTRIB_INDEX    4

GLfloat vertices[] =
{
    -1.0f,  0.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  0.0f,  0.0f
};

GLfloat colors[] =
{
    1.0f,  0.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  0.0f,  1.0f
};

GLfloat vertices2[] =
{
    0.0f,  0.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    1.0f,  0.0f,  0.0f
};

GLuint vao[2];
GLuint vbo[3];

char * load_file(const char *path, long *len)
{
    char *buffer = NULL;
    long err = 0;

    // open binary file for reading
    FILE *fh = fopen(path, "rb");
    if(!fh)
    {
        fprintf(stderr, "fopen: error\n");
        return NULL;
    }

    // fseek to end
    err = fseek(fh, 0L, SEEK_END);
    if(err != 0)
    {
        fprintf(stderr, "fseek: error\n");
        return NULL;
    }

    // get file size
    *len = ftell(fh);
    if(*len == -1L)
    {
        fprintf(stderr, "ftell: error\n");
        return NULL;
    }

    // fseek to start
    err = fseek(fh, 0L, SEEK_SET);
    if(err != 0)
    {
        fprintf(stderr, "fseek: error\n");
        return NULL;
    }

    // allocate buffer
    buffer = (char *)malloc(*len);
    if(buffer == NULL)
    {
        fprintf(stderr, "malloc: error\n");
        return NULL;
    }

    // read into buffer
    err = fread(buffer, *len, 1, fh);
    if(err != 1)
    {
        fprintf(stderr, "fread: error\n");
        return NULL;
    }

    // close file
    err = fclose(fh);
    if(err == EOF) fprintf(stderr, "fclose: error\n");

    return buffer;
}

SDL_Window * setup_rc(SDL_GLContext *ctx)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL_Init: error: %s\n", SDL_GetError());
        return NULL;
    }

    // create dummy window and context to test default options

    SDL_Window *wnd = SDL_CreateWindow("", 0, 0, 0, 0, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
    if(!wnd)
    {
        fprintf(stderr, "SDL_CreateWindow: dummy error: %s\n", SDL_GetError());
        return NULL;
    }

    *ctx = SDL_GL_CreateContext(wnd);
    if(!*ctx)
    {
        fprintf(stderr, "SDL_GL_CreateContext: dummy error: %s\n", SDL_GetError());
        return NULL;
    }

    // debug info

    fprintf(stderr, "[DUMMY] OpenGL version: %s\n", glGetString(GL_VERSION));
    fprintf(stderr, "[DUMMY] GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    SDL_GL_DeleteContext(*ctx);
    SDL_DestroyWindow(wnd);

    // set attributes and create window and context

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    wnd = SDL_CreateWindow("AGPLab1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                       1280, 720,
                                       SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
                                       | SDL_WINDOW_BORDERLESS);
    if(!wnd)
    {
        fprintf(stderr, "SDL_CreateWindow: error: %s\n", SDL_GetError());
        return NULL;
    }

    *ctx = SDL_GL_CreateContext(wnd);
    if(!*ctx)
    {
        fprintf(stderr, "SDL_GL_CreateContext: error: %s\n", SDL_GetError());
        return NULL;
    }

    // debug info

    fprintf(stderr, "OpenGL version: %s\n", glGetString(GL_VERSION));
    fprintf(stderr, "GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // enable vsync
    SDL_GL_SetSwapInterval(1);

    return wnd;
}

void print_shader_error(GLint shader)
{
    GLint max_len = 0;
    GLint log_len = 0;
    unsigned char is_shader = glIsShader(shader);

    if(is_shader) glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_len);
    else glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &max_len);

    if(max_len > 0)
    {
        GLchar *message = (GLchar *)malloc(sizeof(GLchar) * max_len);

        if(is_shader) glGetShaderInfoLog(shader, max_len, &log_len, message);
        else glGetProgramInfoLog(shader, max_len, &log_len, message);

        fprintf(stderr, "%s\n", message);
        free(message);
    }
}

GLuint init_shaders(const char *v_path, const char *f_path)
{
    GLuint v_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint f_id = glCreateShader(GL_FRAGMENT_SHADER);

    long v_len, f_len;
    const char *v_src = load_file(v_path, &v_len);
    const char *f_src = load_file(f_path, &f_len);

    glShaderSource(f_id, 1, &f_src, (GLint *)&f_len);
    glShaderSource(v_id, 1, &v_src, (GLint *)&v_len);

    GLint compile_status;

    glCompileShader(v_id);
    glGetShaderiv(v_id, GL_COMPILE_STATUS, &compile_status);
    if(!compile_status)
    {
        fprintf(stderr, "glCompileShader(v_id): error\n");
        print_shader_error(v_id);
    }

    glCompileShader(f_id);
    glGetShaderiv(f_id, GL_COMPILE_STATUS, &compile_status);
    if(!compile_status)
    {
        fprintf(stderr, "glCompileShader(f_id): error\n");
        print_shader_error(f_id);
    }

    GLuint p_id = glCreateProgram();
    glAttachShader(p_id, v_id);
    glAttachShader(p_id, f_id);

    glBindAttribLocation(p_id, ATTRIB_VERTEX, "a_vVertex");
    glBindAttribLocation(p_id, ATTRIB_COLOR, "a_vColor");
    glBindAttribLocation(p_id, ATTRIB_NORMAL, "a_vNormal");
    glBindAttribLocation(p_id, ATTRIB_TEXCOORD, "a_vTexCoord");

    glLinkProgram(p_id);
    glUseProgram(p_id);

    free((void *)v_src);
    free((void *)f_src);

    return p_id;
}

void init(void)
{
    init_shaders("minimal.vsh", "minimal.fsh");

    glGenVertexArrays(1, &vao[0]);
    glBindVertexArray(vao[0]);

    glGenBuffers(1, &vbo[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(ATTRIB_VERTEX);

    glGenBuffers(1, &vbo[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(float), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(ATTRIB_COLOR, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(ATTRIB_COLOR);

    glGenVertexArrays(1, &vao[1]);
    glBindVertexArray(vao[1]);

    glGenBuffers(1, &vbo[2]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(float), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(ATTRIB_VERTEX);

    glBindVertexArray(vao[0]);
}

void draw(SDL_Window *wnd)
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vao[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(vao[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // if core profile is active, should not render anything
    glColor3f(0.5f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.5f, 0.5f, 0.0f);
    glVertex3f(0.7f, 0.5f, 0.0f);
    glVertex3f(0.5f, 0.7f, 0.0f);

    SDL_GL_SwapWindow(wnd);
}

int main(int argc, const char ** argv)
{
    SDL_GLContext ctx;
    SDL_Window *wnd = setup_rc(&ctx);
    if(!wnd)
    {
        fprintf(stderr, "setup_rc: error\n");
        return 1;
    }

#if defined(_WIN32) || defined(__linux__)
    glewExperimental = true;

    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
        fprintf(stderr, "glewInit: error\n");
        return 1;
    }
#endif

    init();

    SDL_Event e;
    for(unsigned char running=1; running;)
    {
        while(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
                case SDL_QUIT:
                {
                    running = 0;
                    break;
                }
                case SDL_KEYDOWN:
                {
                    switch(e.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            running = 0;
                            break;
                    }
                    break;
                }
            }
        }

        draw(wnd);
    }

    SDL_DestroyWindow(wnd);
    SDL_Quit();

    return 0;
}
