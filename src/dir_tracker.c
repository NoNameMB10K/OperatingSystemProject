#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/wait.h>

#include "dir_tracker.h"
#include "path_dt.h"
#include "memory_checks.h"
#include "file_operations.h"
#include "process_manager.h"
#include "list.h"

bool has_rights(struct stat i_node)
{
    return !(!(i_node.st_mode & 777));
}

void get_curent_file_info(Path_DT x, char *dest, int depth, int indent)
{
    dest[0] = '\0';
    for(int i = 1; i <= depth*indent; i ++)
    {
        dest[i-1] = ' ';
    }
    dest[depth * indent] = '\0';

    // printf("line:[%s]\n", dest);

    char aux_read_data[MAX_DATA_SIZE];

    sprintf(aux_read_data, 
    ", %ld, %d, %d, %ld, %ld, ",
    x.i_node.st_ino, x.i_node.st_mode,
    x.i_node.st_uid, x.i_node.st_size,
    x.i_node.st_ino);

    strcat(dest, aux_read_data);
    // printf("line:%s\n", dest);
    strcat(dest, x.fileName);
    // printf("line:%s\n", dest);
    strcat(dest, "\n");
    // printf("line:%s", dest);
}

int get_snapshot(Path_DT current_dir, int depth, int indent, char **snap, char *path_to_sh, char *ISOLATED_SPACE_DIR, Node **head)
{
    int nr_processed = 0;
    char line[MAX_DATA_SIZE + MAX_INDEX_SIZE];
    get_curent_file_info(current_dir, line, depth, indent);
    // printf("RESULT:%s", line);
    if(*snap == NULL)
    {
        *snap = (char *)realloc(*snap, (strlen(line) + 3) * sizeof(char));
        (*snap)[0] = '\0'; 
    }
    else
        *snap = (char *)realloc(*snap, (strlen(line) + strlen(*snap) + 3) * sizeof(char));
    is_null(*snap, "snapshot realocating memory run into an error\n");
    strcat(*snap, line);
    // printf("result%s\n\n\n",*snap);


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
            nr_processed += get_snapshot(entry, depth + 1, indent, snap, path_to_sh, ISOLATED_SPACE_DIR, head);
        else
        { 
            if(has_rights(entry.i_node))
            {   
                get_curent_file_info(entry, line, depth + 1, indent);
                *snap = (char *)realloc(*snap, (strlen(line) + strlen(*snap) + 3) * sizeof(char));
                is_null(*snap, "snapshot realocating memory run into an error\n");
                strcat(*snap, line);
            }
            else
            {
                nr_processed ++;
                int reading_pipe = 0;
                execute_shell_script(entry.fullPath, entry.fileName, path_to_sh, ISOLATED_SPACE_DIR, &reading_pipe);
                insert_data(head, reading_pipe, 0, "");
            }
        }
        first_entry = readdir(directory);
    }
    return nr_processed;
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

void save_snapshot(Path_DT cache_file_csv, char *text)
{
    int fd_file_csv = open_snapshot_file_write(cache_file_csv.fullPath);
    int nr_of_bytes = strlen(text) * sizeof(char);

    // printf("[%s]\n", text);
    if(write(fd_file_csv, text, nr_of_bytes) != nr_of_bytes)
    {
        perror("In write cahche file\n");
        free(text);
        exit(EXIT_FAILURE);
    }
    
    if(close(fd_file_csv) < 0)
    {
        perror("Couldn t close file after writing in it\n");
        free(text);
        exit(EXIT_FAILURE);
    }   
}

void track(Path_DT father, char *CACHE_DIR, char *path_to_sh,  char *ISOLATED_SPACE_DIR)
{
    Path_DT cache_file_csv = make_cache_file_path(CACHE_DIR, father, CACHE_FILE_EXTENSION);

    Node *head = NULL;
    char *text = NULL;
    int actual_viruses = 0;
    int processes_created = get_snapshot(father, 0, INDENT, &text, path_to_sh, ISOLATED_SPACE_DIR, &head);
    

    //read from pipes
    Node *son = head;
    while(son != NULL)
    {
        char response[FULL_PATH_LENGHT];
        
        int nr_of_read_bytes = read(son->read_int_pipe, response, FULL_PATH_LENGHT);
        response[nr_of_read_bytes - 1] = '\0';// -1 pt ca eu trimit "string\n"
        if(nr_of_read_bytes < 0)
            exit(EXIT_FAILURE);
            
        // printf("RESPONSE: [%s]\nresp len=%ld\n", response, strlen(response));

        if(strcmp(response, "SAFE") != 0)
        {
            // printf("NSFW %s\n", name_file);
            char dest[FULL_PATH_LENGHT];
            strcpy(dest, ISOLATED_SPACE_DIR);

            char *last_slash = strrchr(response, '/');
            char name_file[100];
            strcpy(name_file, last_slash);
            strcat(dest, name_file);

            if (rename(response, dest) != 0) { 
                if (remove(response) != 0) {
                    perror("Error deleting original file");
                    exit(EXIT_FAILURE);
                }
                perror("Error moving file");
                exit(EXIT_FAILURE);
            }
            actual_viruses ++;
        }
        
        close(son->read_int_pipe);

        son = son ->next;
    }
    

    //wait for processes to finish
    int return_code = -1;
    pid_t finished_pid = 0;
    for(int i = 1; i <= processes_created; i ++)
    {
        finished_pid = wait(&return_code);
        if(WIFEXITED(return_code))
            if(WEXITSTATUS(return_code) != EXIT_SUCCESS)
                printf("wait pid=%d: code=%d (not happy code)\n", finished_pid, WEXITSTATUS(return_code)); 
    }

    char *loaded_text = NULL;
    load_snapshot(&loaded_text, cache_file_csv);
    if(strcmp(text, loaded_text) == 0)
        printf("No changes found for %s\nWere found %d suspicious files:%d were viruses\n",
        father.fileName, processes_created, actual_viruses);
    else
    {
        printf("Changes found for %s, a new version was saved\nWere found %d suspicious files:%d were viruses\n",
        father.fileName, processes_created, actual_viruses);
    }
    
    save_snapshot(cache_file_csv, text);

    free(loaded_text);
    free(text);
}


