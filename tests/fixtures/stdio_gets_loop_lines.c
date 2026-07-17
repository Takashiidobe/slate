#include <stdio.h>

int main(void) {
    remove("slate_stdio_gets_loop_lines.tmp");
    FILE *f = fopen("slate_stdio_gets_loop_lines.tmp", "w");
    if (!f) {
        puts("open-fail");
        return 0;
    }
    fputs("first\n", f);
    fputs("second\n", f);
    fputs("third", f);
    fclose(f);

    FILE *g = fopen("slate_stdio_gets_loop_lines.tmp", "r");
    if (!g) {
        puts("reopen-fail");
        return 0;
    }
    char line[64];
    while (fgets(line, sizeof line, g) != NULL) {
        fputs(line, stdout);
    }
    fclose(g);
    remove("slate_stdio_gets_loop_lines.tmp");
    return 0;
}
