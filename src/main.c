#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include <stdbool.h>

#include "memory_checks.h"
#include "path_dt.h"
#include "dir_tracker.h"

void set_flags(int argc, char *argv[], char **CACHE_DIR, int *start, int *end, char **ISOLATED_SPACE_DIR)
{
    int found_cahe_dir = 0, found_isolated_space_dir = 0;
    *start = -1;
    *end = -1;
    int before_first_flag = -1;
    for(int i = 1; i < argc - 1; i ++)
    {
        if(!(argv[i][0] == '-'))
            continue;

        before_first_flag = (before_first_flag == -1) ? i : before_first_flag;

        if(argv[i][1] == 'O' || argv[i][1] == 'o') // -o -O flag
        {
            found_cahe_dir = 1;
            *CACHE_DIR = (char *)malloc(strlen(argv[i + 1]) * sizeof(char)); 
            is_null(*CACHE_DIR, ALOC_TEXT);
            strcpy(*CACHE_DIR, argv[i + 1]);
            i ++;
        }

       if(argv[i][1] == 'X' || argv[i][1] == 'x') // -x -X flag
        {
            found_isolated_space_dir = 1;
            *ISOLATED_SPACE_DIR = (char *)malloc(strlen(argv[i + 1]) * sizeof(char)); 
            is_null(*ISOLATED_SPACE_DIR, ALOC_TEXT);
            strcpy(*ISOLATED_SPACE_DIR, argv[i + 1]);
            i ++;
        }

        if(*start != -1 && *end == -1)//found another flag aftre start, it s last index with  a dir
            *end = i - 1;

        if(argv[i][1] == 'd' || argv[i][1] == 'D') // -d -D flag
            *start = i + 1;
    }

    if(!found_cahe_dir)
    {
        *CACHE_DIR = (char *)malloc(16 * sizeof(char)); 
        is_null(*CACHE_DIR, ALOC_TEXT);
        strcpy(*CACHE_DIR, "FileSaverCache");
    }

    if(found_isolated_space_dir == 0)
    {
        *ISOLATED_SPACE_DIR = (char *)malloc(30 * sizeof(char)); 
        is_null(*ISOLATED_SPACE_DIR, ALOC_TEXT);
        strcpy(*ISOLATED_SPACE_DIR, "IsolatedSpaceDirectory");
    }

    
    if(*start == -1)
    {
        *start = 1;
        *end = (before_first_flag == -1) ? argc - 1 : before_first_flag;
    }
    if(*end == -1)
        *end = argc;
}


int main(int argc, char *argv[])
{
    int start, end;// start < end
    char *CACHE_DIR = NULL; 
    char *ISOLATED_SPACE_DIR = NULL;

    set_flags(argc, argv, &CACHE_DIR, &start, &end, &ISOLATED_SPACE_DIR);
    for(int i  = start; i < end; i ++)
    {
        bool exists;
        Path_DT x = make_path(argv[i], &exists);
        char *text = NULL;
        get_snapshot(x, 0, INDENT, &text);
        printf("[%s]\n", text);
    }
    return 0;
}