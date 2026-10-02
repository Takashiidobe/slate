#include <stdio.h>
struct common { volatile int value; };
extern int project_get(struct common *p);
int main(void) {
    struct common value = {27};
    printf("%d\n", project_get(&value));
    return 0;
}
