#include_next <stddef.h>

int main(void) { return sizeof(size_t) == sizeof(void *) ? 0 : 1; }
