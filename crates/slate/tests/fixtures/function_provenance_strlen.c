#include <stdio.h>

size_t strlen(const char *s) { return 5; }

size_t invoke(const char *s) { return strlen(s); }

size_t invoke_indirect(const char *s) {
    size_t (*call)(const char *) = strlen;
    return call(s);
}

int main(void) {
    printf("%zu %zu\n", invoke("hello world"), invoke_indirect("hello world"));
    return 0;
}
