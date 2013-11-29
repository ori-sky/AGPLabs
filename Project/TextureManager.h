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

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

class TextureManager
{
public:
    static GLuint LoadBMP(const char *path, GLenum texture_unit, GLfloat aniso)
    {
        GLuint id;
        SDL_Surface *texture = SDL_LoadBMP(path);

        glActiveTexture(texture_unit);

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);

        GLenum format = 0;
        switch(texture->format->BytesPerPixel)
        {
            case 3:
            format = texture->format->Rmask == 0xFF ? GL_RGB : GL_BGR;
            break;
            case 4:
            format = texture->format->Rmask == 0xFF ? GL_RGBA : GL_BGRA;
            break;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->w, texture->h, 0, format,
                     GL_UNSIGNED_BYTE, texture->pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        SDL_FreeSurface(texture);
        return id;
    }
};

#endif
