#include <stdio.h>

#if defined(PROJECT_FEATURE_X)
static int feature(void) {
    return 1;
}
#else
static int feature(void) {
    return 0;
}
#endif

int main(void) {
    printf("%d\n", feature());
    return 0;
}
