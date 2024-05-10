#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "error_checks.h"


Node* create_node(int read_int_pipe, int pid, const char* text)
{
    Node *new_node = (Node*)malloc(sizeof(Node));
    is_null(new_node, "Failed to allocate memory for new node\n");

    new_node->read_int_pipe = read_int_pipe;
    new_node->pid = pid;
    strncpy(new_node->text, text, sizeof(new_node->text));
    new_node->text[sizeof(new_node->text) - 1] = '\0';
    new_node->next = NULL;

    return new_node;
}

void insert_node(Node **head, Node *node)
{
    if (node == NULL) return;
    node->next = *head;
    *head = node;  
}

void insert_data(Node **head, int read_int_pipe, int pid, const char* text)
{
    Node *node = create_node(read_int_pipe, pid, text);
    insert_node(head, node);
}

void free_list(Node **head)
{
    Node *current = *head;
    Node *next_node = NULL;

    while (current != NULL) {
        next_node = current->next;  
        free(current);              
        current = next_node;        
    }

    *head = NULL;  
}
