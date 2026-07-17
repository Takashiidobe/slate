#include <stdio.h>

int main(void) {
    remove("slate_stdio_gets_loop_eof.tmp");
    FILE *f = fopen("slate_stdio_gets_loop_eof.tmp", "w");
    if (!f) {
        puts("open-fail");
        return 0;
    }
    fputs("only\n", f);
    fclose(f);

    FILE *g = fopen("slate_stdio_gets_loop_eof.tmp", "r");
    if (!g) {
        puts("reopen-fail");
        return 0;
    }
    char line[64];
    while (fgets(line, sizeof line, g) != NULL) {
        fputs(line, stdout);
    }
    fclose(g);
    puts("done");

    FILE *h = fopen("slate_stdio_gets_loop_eof.tmp", "w");
    if (!h) {
        puts("open-fail");
        return 0;
    }
    fclose(h);

    FILE *e = fopen("slate_stdio_gets_loop_eof.tmp", "r");
    if (!e) {
        puts("reopen-fail");
        return 0;
    }
    char empty_line[64];
    while (fgets(empty_line, sizeof empty_line, e) != NULL) {
        fputs(empty_line, stdout);
    }
    fclose(e);
    puts("empty-done");
    remove("slate_stdio_gets_loop_eof.tmp");
    return 0;
}
