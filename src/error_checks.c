#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void is_lt_zero(int test_val, const char *text)
{
    if(test_val < 0) {
        perror(text);
        exit(EXIT_FAILURE);
    }
}

void is_null(void* ptr, const char *text) {
    if (ptr == NULL) {
        perror(text);
        exit(EXIT_FAILURE);
    }
}