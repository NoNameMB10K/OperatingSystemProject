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
            get_curent_file_info(entry, line, depth + 1, indent);
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

void load_snapshot(char **loaded_text, Path_DT cache_file_csv)
{
    int text_size = LOAD_CHUNK_SIZE + 1;

    char *text = (char *)malloc((sizeof(char) + 1)* LOAD_CHUNK_SIZE);
    char *chunk = (char *)malloc((sizeof(char) + 1)* LOAD_CHUNK_SIZE);

    is_null(text, ALOC_TEXT);
    is_null(chunk, ALOC_TEXT);

    text[0] = '\0';

    int fd_file_csv = open_snapshot_file_read(cache_file_csv.fullPath);
    int read_status = read(fd_file_csv, chunk, LOAD_CHUNK_SIZE);
    while(read_status == LOAD_CHUNK_SIZE)
    {
        chunk[LOAD_CHUNK_SIZE] = '\0';
        strcat(text, chunk);
        text_size += LOAD_CHUNK_SIZE;
        char *temp1 = (char *)realloc(text, text_size*sizeof(char));
        is_null(temp1, REALOC_TEXT);
        text = temp1;
        read_status = read(fd_file_csv, chunk, LOAD_CHUNK_SIZE);
    }
    if(read_status < 0)
    {
        perror("Error during loading data from file.csv\n");
        free(chunk);
        free(text);
        exit(EXIT_FAILURE);
    }
    if(read_status < LOAD_CHUNK_SIZE)
    {
        chunk[read_status] = '\0';
        strcat(text, chunk);
    }
    free(chunk);
    *loaded_text = text;

    if(close(fd_file_csv) < 0)
    {
        perror("Couldn t close file.csv after loading data\n");
        free(text);
        exit(EXIT_FAILURE);
    }
}


void save_snapshot(char *dir_path, char *CACHE_DIR)
{
    bool exists;
    Path_DT father = make_path(dir_path, &exists);
    if(exists == false)
        return;
    char *text = NULL;
    get_snapshot(father, 0, INDENT, &text);
    
    Path_DT cache_file_csv = make_cache_file_path(CACHE_DIR, father, CACHE_FILE_EXTENSION);
    char *loaded_text = NULL;
    load_snapshot(&loaded_text, cache_file_csv);
    if(strcmp(text, loaded_text) == 0)
        printf("No changes found for %s\n", father.fileName);
    else
        printf("Changes found for %s, a new version was saved\n", father.fileName);
    
    int fd_file_csv = open_snapshot_file_write(cache_file_csv.fullPath);
    int nr_of_bytes = strlen(text) * sizeof(char);
    if(write(fd_file_csv, text, nr_of_bytes) != nr_of_bytes)
    {
        perror("in write cahche file\n");
        free(loaded_text);
        free(text);
        exit(EXIT_FAILURE);
    }
    
    if(close(fd_file_csv) < 0)
    {
        perror("dupa ce s o scris in file nu s o inchis\n");
        free(loaded_text);
        free(text);
        exit(EXIT_FAILURE);
    }   

    free(loaded_text);
    free(text);
}




