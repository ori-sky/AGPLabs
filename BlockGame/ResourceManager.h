#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "stdio.h"
#include "stdlib.h"

class ResourceManager
{
public:
    static char * Load(const char *path, long *len);
};

#endif
