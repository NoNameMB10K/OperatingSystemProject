#ifndef PATH_DATA_TYPE
    #define PATH_DATA_TYPE

    #define FULL_PATH_LENGHT 500
    #define NAME_LENGHT 50

    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <stdbool.h>
    typedef struct type{
        char fullPath[FULL_PATH_LENGHT];
        char fileName[NAME_LENGHT];
        struct stat i_node;
    }Path_DT;

    Path_DT make_path(char *path, bool *exists);
    void get_ino_string(Path_DT x, char *dest);
    void get_path_to_cache(Path_DT x, char *cache_dir, char *dest);
    Path_DT make_subdir_path(Path_DT father, char *child_name);
#endif