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
//pt open
#include <fcntl.h>

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
        perror("Directorul nu a putut sa fie deschis\n");
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

int open_snapshot_file_write(char *p)
{
    //O_APPEND
    int ans = open(p, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP);
    if(ans < 0)
    {
        printf("Error opening a file .csv, func :open snapshot file\n");
        exit(EXIT_FAILURE);
    }
    return  ans;
}

int open_snapshot_file_read(char *p)
{
    int ans = open(p, O_RDONLY);
    if(ans < 0)
    {
        perror("Error opening file csv fo loading data (in read mode)\n");
        exit(EXIT_FAILURE);
    }
    return  ans;
}
