#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
//for fork
#include <sys/types.h>
#include <unistd.h>


#include "path_dt.h"
#include "dir_tracker.h"
#include "error_checks.h"

void generate_traking_process(char *dir_path, char *CACHE_DIR, char *path_to_sh, char *ISOLATED_SPACE_DIR)
{
    bool exists;
    Path_DT father = make_path(dir_path, &exists);
    if(exists == false)
    {
        printf("%s is not a file and was skipped\n", father.fullPath);
        return;
    }

    pid_t pid_process = fork();
    is_lt_zero(pid_process, "Traking process fork failed\n");
    if(pid_process == 0)//sunt in copil
    {
        track(father, CACHE_DIR, path_to_sh, ISOLATED_SPACE_DIR);
        exit(EXIT_SUCCESS);
    }
    else //sunt in parinte
    {
        return;
    }
}

void execute_shell_script(char *path_file_with_no_rights, char *name_file, char *path_to_sh_script, char *ISOLATED_SPACE_DIR, int *read_pipe)
{

    int pfd[2];
    is_lt_zero(pipe(pfd), "Pipe couldn t be created\n");


    pid_t pid_process = fork();
    is_lt_zero(pid_process, "Executing sh failed\n");
    if(pid_process == 0)//sunt in copil
    {
        close(pfd[0]);/* inchide capatul de citire; */
        dup2(pfd[1],1);
        execl(path_to_sh_script, "FS", path_file_with_no_rights, ISOLATED_SPACE_DIR,(char *)NULL);
        
        close(pfd[1]);
        perror("Exec didn t overwrite\n");
        exit(EXIT_FAILURE);
    }
    else //sunt in parinte
    {
        *read_pipe = pfd[0];
        close(pfd[1]);/* inchide capatul de scriere */
        return;
    }
}