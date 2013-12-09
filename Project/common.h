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

#ifndef COMMON_H
#define COMMON_H

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
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#define ASSERT_GL(CALL) \
    glGetError(); \
    CALL; \
    { \
        GLenum _err_ = glGetError(); \
        if(_err_ != GL_NO_ERROR) fprintf(stderr, "OpenGL error 0x%x\n  » %s\n    » %s\n", _err_, GAME_DOMAIN, #CALL); \
    }

#if defined(_WIN32) || defined(_WIN64)
#include <minmax.h>
#define fmax max
#define fmin min
#undef main
#endif

#endif
