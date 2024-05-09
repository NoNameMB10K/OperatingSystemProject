#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

#include "path_dt.h"
#include "file_operations.h"
#include "error_checks.h"

Path_DT make_path(char *path, bool *exists)
{
    Path_DT temp;
    temp.fileName[0] = '\0';
    temp.fullPath[0] = '\0';

    int path_len = strlen(path);
    if(path[path_len-1] == '/')
        path[path_len - 1] = '\0';

    bool path_exists;
    temp.i_node = get_i_node(path, &path_exists);

    if(!path_exists)
    {
        *exists = false; 
        strcpy(temp.fullPath, path);
        strcpy(temp.fileName, path);
        return temp;
    }

    char pwd[FULL_PATH_LENGHT];
    if (getcwd(pwd, FULL_PATH_LENGHT) == NULL)
    {
        perror("couldn t get pwd");
        exit(EXIT_FAILURE);
    }
    
    int pwd_len = strlen(pwd);
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

    *exists = true;
    return temp;
}

Path_DT make_subdir_path(Path_DT father, char *child_name)
{
    Path_DT temp;
    strcpy(temp.fullPath, father.fullPath);
    strcat(temp.fullPath, "/");
    strcat(temp.fullPath, child_name);

    // printf("father:[%s]\nchild:[%s]\nch full:[%s]\n",
    // father.fullPath, child_name, temp.fullPath);

    bool path_exists;
    temp.i_node = get_i_node(temp.fullPath, &path_exists);

    if(!path_exists)
    {
        perror("given path OF CHILD does not exist");
        exit(EXIT_FAILURE);
    }

    //from full path i get the name
    char *last_slash = strrchr(temp.fullPath, '/');
    strcpy(temp.fileName, last_slash);

    return temp;
}

Path_DT make_cache_file_path(char *CACHE_DIR, Path_DT father, char *cache_file_extension)
{
    //make path to file
    char  dir_path_to_save[FULL_PATH_LENGHT];
    strcpy(dir_path_to_save, CACHE_DIR);
    strcat(dir_path_to_save, "/");
    char ino[10];
    get_ino_string(father, ino);
    strcat(dir_path_to_save, ino);
    strcat(dir_path_to_save, cache_file_extension);
    
    bool exists;

    //chck if cache dir exists
    struct stat i_node = get_i_node(CACHE_DIR, &exists);
    if(exists == false || S_ISDIR(i_node.st_mode) != 1)
    {
        //printf("is dir:%d\ncahceDir:%s\n", S_ISDIR(i_node.st_mode), CACHE_DIR);
        if(S_ISDIR(i_node.st_mode) != 1 && exists == true)
            is_lt_zero(remove(CACHE_DIR), "Cache was not a dir and couldn t be deleted\n");
        mkdir(CACHE_DIR, 0700);
    }   

    //check if cache file.csv exists
    i_node = get_i_node(dir_path_to_save, &exists);
    if(exists == false || S_ISREG(i_node.st_mode) != 1)
    {
        //printf("is file:%d\ncahceFile:%s\n", S_ISREG(i_node.st_mode), dir_path_to_save);
        if(S_ISREG(i_node.st_mode) != 1 && exists == true)
            is_lt_zero(remove(dir_path_to_save), "Cache file was not a file and couldn t be deleted\n");
        int fd= open_snapshot_file_write(dir_path_to_save);
        is_lt_zero(fd,"Couldn t propperly close file.csv\n"); 
    }  

    Path_DT temp;
    strcpy(temp.fullPath, dir_path_to_save);
    temp.fileName[0] = '\0';

    return temp;
}


void get_ino_string(Path_DT x, char *dest)
{
    char temp[10];
    sprintf(temp, "%ld", x.i_node.st_ino);
    strcpy(dest, temp);
}

void get_path_to_cache(Path_DT x, char *cache_dir, char *dest)
{
    int cache_dir_len = strlen(cache_dir);
    if(cache_dir[cache_dir_len-1] == '/')
        cache_dir[cache_dir_len - 1] = '\0';

    char temp[FULL_PATH_LENGHT];
    char ino[10];
    get_ino_string(x, ino);

    temp[0] = '\0';
    strcpy(temp, cache_dir);
    strcat(temp, "/");
    strcat(temp, ino);
    strcat(temp, ".csv");

    strcpy (dest, temp);
}

