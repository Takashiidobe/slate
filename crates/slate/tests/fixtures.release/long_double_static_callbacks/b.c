#include <math.h>
#include <stdio.h>

typedef long double (*unary)(long double);
unary a_step(void);

static long double step(long double x) { return x - 1.25L; }

int main(void) {
    unary steps[] = {a_step(), step, a_step(), cbrtl};
    long double x = 1.0L;
    for (int i = 0; i < 4; i++) {
        x = steps[i](x);
        printf("%.6Lf\n", x);
    }
    printf("distinct %d\n", steps[0] != steps[1]);
    return 0;
}
