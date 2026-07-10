#include <stdio.h>

int main(void) {
    double _Complex a = __builtin_complex(1.0, 2.0);
    double _Complex b = __builtin_complex(3.0, 4.0);
    double _Complex sum = a + b;
    double _Complex diff = a - b;
    printf("%d\n", (int)__real__ sum);
    printf("%d\n", (int)__imag__ sum);
    printf("%d\n", (int)__real__ diff);
    printf("%d\n", (int)__imag__ diff);
    return 0;
}
