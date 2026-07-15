#include <stdio.h>

int main(void) {
    remove("slate_stdio_file_write.tmp");
    FILE *f = fopen("slate_stdio_file_write.tmp", "w");
    if (!f) {
        puts("open-fail");
        return 0;
    }
    fputs("owned\n", f);
    fclose(f);

    FILE *g = fopen("slate_stdio_file_write.tmp", "r");
    if (!g) {
        puts("reopen-fail");
        return 0;
    }
    char buf[16] = {0};
    fgets(buf, sizeof buf, g);
    fclose(g);
    fputs(buf, stdout);
    remove("slate_stdio_file_write.tmp");
    return 0;
}
