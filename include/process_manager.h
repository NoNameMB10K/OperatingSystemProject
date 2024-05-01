#ifndef PROCESS_MANAGER
    #define PROCESS_MANAGER

    void generate_traking_process(char *dir_path, char *CACHE_DIR, char *path_to_sh, char *ISOLATED_SPACE_DIR);
    void execute_shell_script(char *path_file_with_no_rights, char *name_file, char *path_to_sh_script, char *ISOLATED_SPACE_DIR);
#endif