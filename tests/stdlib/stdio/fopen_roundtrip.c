#include <stdio.h>

int main(void) {
    char name[] = "slate_stdio_fopen_roundtrip.tmp";
    remove(name);
    FILE *f = fopen(name, "w+");
    if (!f) {
        puts("open-fail");
        return 0;
    }
    fputs("alpha\n", f);
    fflush(f);
    long pos = ftell(f);
    rewind(f);
    char b[16];
    fgets(b, sizeof b, f);
    printf("%ld:%s", pos, b);
    fclose(f);
    remove(name);
    return 0;
}
