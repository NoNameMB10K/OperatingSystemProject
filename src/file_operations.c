#include "file_operations.h"

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

DIR *open_director(Path_DT p)
{
    DIR *ans = opendir(p.fullPath);
    if(ans == NULL)
    {
        printf("Directorul [%s] nu a putut sa fie deschis\n", p.fullPath);
        exit(EXIT_FAILURE);
    }
    return ans;
}

bool is_dir(Path_DT path)
{   
    return (S_ISDIR(path.i_node.st_mode) != 0);
}

bool is_link(Path_DT path)
{   
    return S_ISLNK(path.i_node.st_mode);
}
