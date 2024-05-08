#ifndef LIST_IMPLEMENTATION
    #define LIST_IMPLEMENTATION

    typedef struct node {
        int read_int_pipe;
        int pid;
        char text[500];
        struct node *next;
    } Node;

    void insert_data(Node **head, int read_int_pipe, int pid, const char* text);
    void insert_node(Node **head, Node *node);
    Node* create_node(int read_int_pipe, int pid, const char* text);
    void free_list(Node **head);

#endif