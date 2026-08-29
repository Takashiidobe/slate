#include <stdio.h>
#include <stdlib.h>

int main(void) {
    const char *s = "-7xyz";
    printf("%d\n", atoi("42abc"));
    printf("%d\n", atoi("  +13 "));
    printf("%d\n", atoi(s));
    printf("%d\n", atoi("nope"));
    printf("%ld\n", atol("1000000000000"));
    printf("%lld\n", atoll("-9000000000000"));
    printf("%.2f\n", atof("  3.14  "));
    printf("%.1f\n", atof("2"));
    printf("%.2f\n", atof("6.5garbage"));
    return 0;
}
