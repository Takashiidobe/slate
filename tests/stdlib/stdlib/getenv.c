#include <stdlib.h>
#include <stdio.h>
int main(void) { char *v = getenv("SLATE_STDLIB_ENV_PROBE"); puts(v ? v : "missing"); return 0; }
