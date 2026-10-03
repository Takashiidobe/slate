#include <stddef.h>

int string2ld(const char *s, size_t slen, long double *dp);
int ld2string(char *buf, size_t len, long double value);
long double ld_add(long double a, long double b);
long double ld_scale(long double value, int factor, long double offset);
