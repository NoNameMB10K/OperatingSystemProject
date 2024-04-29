#ifndef PATH_DATA_TYPE
    #define PATH_DATA_TYPE

    #define FULL_PATH_LENGHT 500
    #define NAME_LENGHT 50

    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
    typedef struct type{
        char fullPath[FULL_PATH_LENGHT];
        char fileName[NAME_LENGHT];
        struct stat i_node;
    }Path_DT;

    Path_DT make_path(const char *path);

#endif