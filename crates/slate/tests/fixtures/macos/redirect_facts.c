#define _DARWIN_C_SOURCE
#include <errno.h>
#include <stdio.h>

FILE *open_darwin_stream(const char *path) { return fopen(path, "r"); }

int darwin_errno(void) { return errno; }

FILE *darwin_stdin(void) { return stdin; }
