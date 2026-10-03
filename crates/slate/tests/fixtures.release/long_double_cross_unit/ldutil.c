#include "ldutil.h"

#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int string2ld(const char *s, size_t slen, long double *dp) {
    char buf[256];
    long double value;
    char *eptr;

    if (slen == 0 || slen >= sizeof(buf)) return 0;
    memcpy(buf, s, slen);
    buf[slen] = '\0';
    errno = 0;
    value = strtold(buf, &eptr);
    if (isspace(buf[0]) || eptr[0] != '\0' || (size_t)(eptr - buf) != slen ||
        (errno == ERANGE &&
         (value == HUGE_VAL || value == -HUGE_VAL || fpclassify(value) == FP_ZERO)) ||
        errno == EINVAL || isnan(value))
        return 0;
    if (dp) *dp = value;
    return 1;
}

int ld2string(char *buf, size_t len, long double value) {
    size_t l;
    if (isinf(value)) {
        l = (size_t)snprintf(buf, len, "%s", value > 0 ? "inf" : "-inf");
    } else if (isnan(value)) {
        l = (size_t)snprintf(buf, len, "nan");
    } else {
        l = (size_t)snprintf(buf, len, "%.17Lf", value);
        if (strchr(buf, '.') != NULL) {
            char *p = buf + l - 1;
            while (*p == '0') {
                p--;
                l--;
            }
            if (*p == '.') l--;
        }
        buf[l] = '\0';
    }
    return (int)l;
}

long double ld_add(long double a, long double b) { return a + b; }

long double ld_scale(long double value, int factor, long double offset) {
    return value * factor + offset;
}
