#include <stdio.h>

enum Basic {
    BasicZero,
    BasicOne,
    BasicFive = 5,
    BasicSix,
    BasicNegative = -2,
    BasicNegativeNext
};

int main(void) {
    printf("%d\n", BasicZero);
    printf("%d\n", BasicOne);
    printf("%d\n", BasicFive);
    printf("%d\n", BasicSix);
    printf("%d\n", BasicNegative);
    printf("%d\n", BasicNegativeNext);
    return 0;
}
