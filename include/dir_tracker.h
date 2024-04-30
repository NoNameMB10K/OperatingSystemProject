#ifndef DIR_TRACKER
    #define DIR_TRACKER

    #define INDENT 4
    #define MAX_DATA_SIZE 1000
    #define MAX_INDEX_SIZE INDENT*10
    #define CACHE_FILE_EXTENSION ".csv"
    #define LOAD_CHUNK_SIZE 100
    
    #include "path_dt.h"
    
    void track(Path_DT father, char *CACHE_DIR, char *path_to_sh,  char *ISOLATED_SPACE_DIR);
#endif