#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
//for fork
#include <sys/types.h>
#include <unistd.h>


#include "path_dt.h"
#include "dir_tracker.h"

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

void execute_shell_script(char *path_file_with_no_rights, char *path_to_sh_script, char *ISOLATED_SPACE_DIR)
{
    pid_t pid_process = fork();
    if(pid_process == 0)//sunt in copil
    {
        execl(path_to_sh_script, "FS", path_file_with_no_rights, ISOLATED_SPACE_DIR,(char *)NULL);
        perror("Exec didn t overwrite\n");
        exit(EXIT_FAILURE);
    }
    else //sunt in parinte
    {
        return;
    }
}