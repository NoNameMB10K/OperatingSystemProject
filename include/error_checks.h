#ifndef ERROR_CHECKS_H
    #define ERROR_CHECKS_H
    #define ALOC_TEXT "allocating memory error"
    #define REALOC_TEXT "reallocating  memory error"

    void is_null(void* ptr, const char *text);
    void is_lt_zero(int test_val, const char *text);
#endif