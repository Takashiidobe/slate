#include <stdio.h>

int main(void) {
    char a[] = "slate_stdio_rename_a.tmp";
    char b[] = "slate_stdio_rename_b.tmp";
    remove(a);
    remove(b);
    FILE *f = fopen(a, "w");
    if (!f) {
        puts("open-fail");
        return 0;
    }
    fputs("R", f);
    fclose(f);
    int renamed = rename(a, b);
    FILE *g = fopen(b, "r");
    int c = g ? fgetc(g) : '?';
    if (g) {
        fclose(g);
    }
    int removed = remove(b);
    printf("%d %c %d\n", renamed, c, removed);
    return 0;
}
