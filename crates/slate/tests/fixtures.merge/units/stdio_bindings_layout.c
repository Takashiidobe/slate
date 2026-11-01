#include <stdio.h>

typedef int (*writer)(int, FILE *);

int stream_layout(void) {
    writer write = fputc;
    write('w', stdout);
    write('\n', stdout);
#ifdef __GLIBC__
    return stdout->_fileno;
#else
    return 1;
#endif
}
