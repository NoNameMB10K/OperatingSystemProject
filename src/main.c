#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "process_manager.h"
#include "error_checks.h"

#define MAX_NR_THREADS 10

/*
TO DO
de tratat erori la fork
*/
void set_flags(int argc, char *argv[], char **CACHE_DIR, int *start, int *end, char **ISOLATED_SPACE_DIR, char **PATH_SH)
{
    int found_cahe_dir = 0, found_isolated_space_dir = 0, found_path_sh = 0;
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

       if(argv[i][1] == 'S' || argv[i][1] == 's') // -s -S flag
        {
            found_path_sh = 1;
            *PATH_SH = (char *)malloc(strlen(argv[i + 1]) * sizeof(char)); 
            is_null(*PATH_SH, ALOC_TEXT);
            strcpy(*PATH_SH, argv[i + 1]);
            i ++;
        }

        if(*start != -1 && *end == -1)//found another flag aftre start, it s last index with  a dir
            *end = i - 1;

        if(argv[i][1] == 'd' || argv[i][1] == 'D') // -d -D flag
            *start = i + 1;
    }

    if(!found_path_sh)
    {
        *PATH_SH = (char *)malloc(25 * sizeof(char)); 
        is_null(*PATH_SH, ALOC_TEXT);
        strcpy(*PATH_SH, "verify_for_malicious.sh");
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
    char *PATH_SH = NULL;

    set_flags(argc, argv, &CACHE_DIR, &start, &end, &ISOLATED_SPACE_DIR, &PATH_SH);

    int index_arg = start;
    int live_processes = 0;
    while(index_arg < end)
    {
        live_processes += generate_traking_process(argv[index_arg], CACHE_DIR,PATH_SH, ISOLATED_SPACE_DIR);
        index_arg ++;

        if(live_processes >= MAX_NR_THREADS || index_arg >= end)
        {
            int return_code = -1;
            pid_t finished_pid = 0;
            for(int i = start; i < end; i ++)
            {
                finished_pid = wait(&return_code);
                if(WIFEXITED(return_code))
                    if(WEXITSTATUS(return_code) != EXIT_SUCCESS)
                        printf("Child wait pid=%d: code=%d (not successfuly)\n", finished_pid, WEXITSTATUS(return_code)); 
            }
        }
    }

    return 0;
}