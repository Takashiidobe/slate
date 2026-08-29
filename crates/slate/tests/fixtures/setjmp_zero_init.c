#include <setjmp.h>
#include <stdio.h>

static jmp_buf frame;

int main(void) {
    if (setjmp(frame) == 0) {
        longjmp(frame, 42);
    }
    printf("returned\n");
    return 0;
}
