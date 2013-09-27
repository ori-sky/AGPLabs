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
        fprintf(stderr, "SDL_Init: error\n");
        return NULL;
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    
    SDL_Window *wnd = SDL_CreateWindow("AGPLab1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                       1280, 800,
                                       SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
                                       | SDL_WINDOW_FULLSCREEN_DESKTOP);
    if(!wnd)
    {
        fprintf(stderr, "SDL_CreateWindow: error\n");
        return NULL;
    }
    
    *ctx = SDL_GL_CreateContext(wnd);
    if(!*ctx)
    {
        fprintf(stderr, "SDL_GL_CreateContext: error: %s\n", SDL_GetError());
        return NULL;
    }
    
    // enable vsync
    SDL_GL_SetSwapInterval(1);
    
    return wnd;
}

void printShaderError(const GLint shader) {
	int maxLength = 0;
	int logLength = 0;
	GLchar *logMessage;
    
	// Find out how long the error message is
	if (!glIsShader(shader))
		glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
	else
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
    
	if (maxLength > 0) { // If message has some contents
		logMessage = new GLchar[maxLength];
		if (!glIsShader(shader))
			glGetProgramInfoLog(shader, maxLength, &logLength, logMessage);
		else
			glGetShaderInfoLog(shader,maxLength, &logLength, logMessage);
        
        fprintf(stderr, "%s\n", logMessage);
		delete [] logMessage;
	}
	// should additionally check for OpenGL errors here
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
        printShaderError(v_id);
    }
    
    glCompileShader(f_id);
    glGetShaderiv(f_id, GL_COMPILE_STATUS, &compile_status);
    if(!compile_status)
    {
        fprintf(stderr, "glCompileShader(f_id): error\n");
        printShaderError(f_id);
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

void draw(SDL_Window * window) {
	// clear the screen - good to use grey as all white or all black triangles will still be visible
	glClearColor(0.5f,0.5f,0.5f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
    
	glBindVertexArray(vao[0]);	// Bind mesh VAO
	glDrawArrays(GL_TRIANGLES, 0, 3);	// draw 3 vertices (one triangle)
	
	glBindVertexArray(vao[1]);	// Bind mesh VAO
	glDrawArrays(GL_TRIANGLES, 0, 3);	// draw 3 vertices (one triangle)
    
	// These are deprecated functions. If a core profile has been correctly
	// created, these commands should compile, but wont render anything
	glColor3f(0.5,1.0,1.0);
	glBegin(GL_TRIANGLES);
    glVertex3f(0.5,0.5,0.0);
    glVertex3f(0.7,0.5,0.0);
    glVertex3f(0.5,0.7,0.0);
	glEnd();
    
    SDL_GL_SwapWindow(window); // swap buffers
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
    
#if defined(_WIN32)
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
        SDL_PollEvent(&e);
        
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
        
        draw(wnd);
    }
    
    SDL_DestroyWindow(wnd);
    SDL_Quit();
    
    return 0;
}