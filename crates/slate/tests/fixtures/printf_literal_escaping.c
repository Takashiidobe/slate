#include <stdio.h>

int main(void) {
    int d = 5;
    unsigned h = 0xAB;
    printf("{%d} %% \"quoted\" back\\slash %s|%c|%x\n", d, "hi", 'X', h);
    printf("}}%%{{%d}}\n", d);
    printf("%%%%%d%%%%\n", d);
    printf("{{}}%s{{}}\n", "mid");
    return 0;
}
