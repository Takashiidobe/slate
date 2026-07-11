#include <stdio.h>

int main(void) {
    FILE *f = tmpfile();
    int c = fgetc(f);
    printf("%d %d %d\n", c == EOF ? 1 : 0, feof(f) ? 1 : 0, ferror(f) ? 1 : 0);
    clearerr(f);
    printf("%d %d\n", feof(f) ? 1 : 0, ferror(f) ? 1 : 0);
    fclose(f);
    return 0;
}
