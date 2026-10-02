#include <stddef.h>
#include <stdio.h>
#define OFFSET(T, M) ((size_t)((char *)&((T *)0)->M - (char *)0))
struct Inner { int first; short second; };
struct Record { char prefix; double value; struct Inner nested; unsigned char count; };
int main(void) {
    printf("%zu %zu %zu %zu %zu\n", OFFSET(struct Record, value),
           OFFSET(struct Record, nested), OFFSET(struct Record, nested.second),
           OFFSET(struct Record, count), OFFSET(struct Inner, first));
    return OFFSET(struct Record, nested.second) != offsetof(struct Record, nested.second);
}
