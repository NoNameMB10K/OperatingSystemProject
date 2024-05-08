#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void is_null(void* ptr, const char *text) {
    if (ptr == NULL) {
        perror(text);
        exit(EXIT_FAILURE);
    }
}