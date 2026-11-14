#include "platform.h"

int printf(const char *, ...);

int main(void) {
    printf("%s %d\n", platform_name(), platform_value());
    return 0;
}
