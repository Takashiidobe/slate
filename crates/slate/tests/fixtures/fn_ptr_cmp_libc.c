#include <stdlib.h>

typedef void *(*malloc_fn_t)(size_t);

struct hooks {
    malloc_fn_t malloc_fn;
};

int main(void) {
    struct hooks h;
    h.malloc_fn = malloc;
    int matches = (h.malloc_fn == malloc);
    return matches ? 0 : 1;
}
