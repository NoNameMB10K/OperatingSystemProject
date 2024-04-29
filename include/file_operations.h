#ifndef FILE_OPERATIONS
    #define FILE_OPERATIONS
    #include <dirent.h>
    #include <stdbool.h>
    #include "path_dt.h"

    struct stat get_i_node(const char * path, bool *exists);
    DIR *open_director(Path_DT p);
    bool is_dir(Path_DT path);
    bool is_link(Path_DT path);
#endif