#include <stdio.h>
#include <stdlib.h>

static int values[] = {11, 22, 33, 44, 55};
static const int *middle = &values[(8 - 2) / 3];
static const int *end = values + (2 * 3 - 1);
static int twice(int x) { return x * 2; }
struct Entry {
    const int *value;
    int (*callback)(int);
    void *opaque;
    const char *name;
};
static struct Entry entries[] = {
    {values + (7 - 6), &twice, (void *)&twice, "twice"},
    {&values[1 + 2], abs, (void *)abs, "abs"}
};
static struct {
    struct Entry nested[2];
    const int *tail;
} group = {{{values + (9 / 3), twice, (void *)twice, "group"}}, values + (1 << 2)};

int main(void) {
    static int flag = 7;
    static void *addresses[] = {(void *)&flag, (void *)&twice};
    int (*callback)(int) = (int (*)(int))entries[0].opaque;
    int (*external)(int) = (int (*)(int))entries[1].opaque;
    int (*local)(int) = (int (*)(int))addresses[1];
    printf("%d %ld %d %d %s %d %d %d %d %d %d\n",
           *middle, (long)(end - values), *entries[0].value,
           *entries[1].value, entries[0].name, callback(6), external(-9),
           *group.nested[0].value, *group.tail, *(int *)addresses[0], local(4));
    return group.nested[1].value != 0;
}
