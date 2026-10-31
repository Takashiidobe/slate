#include <stdlib.h>

#ifdef ALPHA
typedef long value_t;
#else
typedef int value_t;
#endif

struct opaque {
    int value;
#ifdef BETA
    int extra;
#endif
};
typedef struct {
    int q;
} *handle;

value_t scale(value_t x) {
    return x * 2;
}

long native_long(long x) {
    return x + (long)sizeof(long);
}

int unprototyped(int x) {
    return x + 1;
}

int table[3] = {1, 2, 3};
int big_unsized[32] = {1, 2, 3};
int big_sized[32] = {1, 2, 3, 4};

const char *label(void) {
    return "ok";
}

static struct opaque shared = {7};

struct opaque *make_opaque(void) {
    return &shared;
}

int read_opaque(struct opaque *opaque) {
    return opaque->value;
}

int read_handle(handle h) {
    return h ? h->q : 9;
}

void quit(int code) {
    exit(code);
}
