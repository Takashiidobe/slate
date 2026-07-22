#include <stdio.h>

int main(void) {
    struct {
        int code;
        const char *message;
        double confidence;
    } error_log[] = {
        {404, "Not Found", 0.99},
        {500, "Internal Server Error", 0.85},
        {200, "OK", 1.00},
    };

    printf("%d\n", error_log[0].code);
    return 0;
}
