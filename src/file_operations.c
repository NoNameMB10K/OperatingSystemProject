#include <stdio.h>
#include<errno.h>
#include<string.h>
#include<ctype.h>
#include <stdlib.h>
#include <stdbool.h>

//system
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

struct stat get_i_node(const char * path, bool * exists)
{
    struct stat i_node;
    if(lstat(path, &i_node) < 0)
    {
        *exists = false;
        return i_node;
    }
    *exists = true;
    return i_node;
}