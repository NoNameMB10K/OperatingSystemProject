#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

#include "path_dt.h"
#include "file_operations.h"

Path_DT make_path(const char *path)
{
    Path_DT temp;
    temp.fileName[0] = '\0';
    temp.fullPath[0] = '\0';

    bool path_exists;
    temp.i_node = get_i_node(path, &path_exists);

    if(!path_exists)
    {
        perror("given path does not exist");
        return temp;
    }

    char pwd[FULL_PATH_LENGHT];
    if (getcwd(pwd, FULL_PATH_LENGHT) == NULL)
    {
        perror("couldn t get pwd");
        exit(EXIT_FAILURE);
    }
    
    int pwd_len = strlen(pwd), path_len = strlen(path);
    char compare_aux[FULL_PATH_LENGHT];
    if(pwd_len < path_len)
    {
        strncpy(compare_aux, path, pwd_len);
        compare_aux[pwd_len] = '\0';
    }
    else
        compare_aux[0] = '\0';

    if(pwd_len >= path_len || strcmp(compare_aux, pwd) != 0)// not full path
    {
        strcpy(temp.fullPath, pwd);
        strcat(temp.fullPath, "/");
    }
    strcat(temp.fullPath, path);

    //from full path i get the name
    char *last_slash = strrchr(temp.fullPath, '/');
    strcpy(temp.fileName, last_slash);
    
    return temp;
}

void get_ino_string(Path_DT x, char *dest)
{
    char temp[10];
    sprintf(temp, "%ld", x.i_node.st_ino);
    strcpy(dest, temp);
}

