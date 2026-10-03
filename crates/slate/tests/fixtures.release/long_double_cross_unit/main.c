#include "ldutil.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

static void incr_by_float(char *stored, const char *increment) {
    long double value = 0, incr;
    if (stored[0] && !string2ld(stored, strlen(stored), &value)) {
        printf("ERR value is not a valid float\n");
        return;
    }
    if (!string2ld(increment, strlen(increment), &incr)) {
        printf("ERR increment is not a valid float\n");
        return;
    }
    value = ld_add(value, incr);
    if (isnan(value) || isinf(value)) {
        printf("ERR increment would produce NaN or Infinity\n");
        return;
    }
    ld2string(stored, 128, value);
    printf("%s\n", stored);
}

int main(void) {
    char stored[128] = "";
    incr_by_float(stored, "10.5");
    incr_by_float(stored, "1.5");
    incr_by_float(stored, "-0.25");
    strcpy(stored, "3");
    incr_by_float(stored, "1.5");
    char buf[128];
    ld2string(buf, sizeof buf, ld_scale(2.5L, 3, 0.125L));
    printf("scale=%s\n", buf);
    return 0;
}
