#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_num(char *s) {
    return atoi(s);
}

int forward_num(char *s) {
    return parse_num(s);
}

int text_len(char *s) {
    return (int)strlen(s);
}

int main(void) {
    char digits[] = "42";
    char word[] = "hello";
    printf("%d %d\n", forward_num(digits), text_len(word));
    return 0;
}
