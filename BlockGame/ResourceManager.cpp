#include "ResourceManager.h"

char * ResourceManager::Load(const char *path, long *len)
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
