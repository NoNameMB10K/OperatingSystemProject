#ifndef DIR_TRACKER
    #define DIR_TRACKER

    #define INDENT 4
    #define MAX_DATA_SIZE 1000
    #define MAX_INDEX_SIZE INDENT*10
    #include "path_dt.h"
    void get_curent_file_info(Path_DT x, char *dest, int depth, int indent);
    void get_snapshot(Path_DT current_dir, int depth, int indent, char **snap);

#endif