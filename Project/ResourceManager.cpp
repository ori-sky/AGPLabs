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

#include "ResourceManager.h"

#include "stdio.h"
#include "stdlib.h"

char * ResourceManager::Load(const char *path, long *len)
{
    char *buffer = NULL;
    long err = 0;

    // open binary file for reading
    FILE *fh = fopen(path, "rb");
    if(!fh)
    {
        fprintf(stderr, "fopen: error opening `%s`\n", path);
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
