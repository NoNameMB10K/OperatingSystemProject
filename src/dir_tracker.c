#include "dir_tracker.h"
#include "path_dt.h"
#include "memory_checks.h"
#include "file_operations.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <dirent.h>

void get_curent_file_info(Path_DT x, char *dest, int depth, int indent)
{
    dest[0] = '\0';
    for(int i = 1; i <= depth*indent; i ++)
    {
        dest[i-1] = ' ';
    }
    dest[depth * indent] = '\0';

    char aux_read_data[MAX_DATA_SIZE];

    sprintf(aux_read_data, 
    ", %ld, %d, %d, %ld, %ld, ",
    x.i_node.st_ino, x.i_node.st_mode,
    x.i_node.st_uid, x.i_node.st_size,
    x.i_node.st_ino);

    strcat(dest, aux_read_data);
    strcat(dest, x.fileName);
    strcat(dest, "\n");
}

void get_snapshot(Path_DT current_dir, int depth, int indent, char **snap)
{
    int children_made = 0;
    char line[MAX_DATA_SIZE + MAX_INDEX_SIZE];
    get_curent_file_info(current_dir, line, depth, indent);

    if(*snap == NULL)
        *snap = (char *)realloc(*snap, (strlen(line) + 3) * sizeof(char));
    else
        *snap = (char *)realloc(*snap, (strlen(line) + strlen(*snap) + 3) * sizeof(char));
    is_null(*snap, "snapshot realocating memory run into an error\n");
    strcat(*snap, line);
    

    DIR *directory = open_director(current_dir);
    struct dirent *first_entry = readdir(directory);
    while(first_entry != NULL)
    {
        if(first_entry->d_name[0] == '.' && (first_entry->d_name[1] == '\0' || (first_entry->d_name[1] == '.' && first_entry->d_name[2] == '\0')))
        {
            //self/dad reference
            first_entry = readdir(directory);
            continue;
        }
        Path_DT entry = make_subdir_path(current_dir, first_entry->d_name);

        if(is_dir(entry))
            get_snapshot(entry, depth + 1, indent, snap);
        else
        { 
            get_curent_file_info(current_dir, line, depth, indent);
            if(*snap == NULL)
                *snap = (char *)realloc(*snap, (strlen(line) + 3) * sizeof(char));
            else
                *snap = (char *)realloc(*snap, (strlen(line) + strlen(*snap) + 3) * sizeof(char));
            is_null(*snap, "snapshot realocating memory run into an error\n");
            strcat(*snap, line);
        }
        first_entry = readdir(directory);
    }
}






