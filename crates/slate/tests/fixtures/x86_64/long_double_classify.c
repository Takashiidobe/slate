#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int string2ld(const char *s, size_t slen, long double *dp) {
  char buf[64];
  char *eptr;
  if (slen == 0 || slen >= sizeof(buf)) return 0;
  memcpy(buf, s, slen);
  buf[slen] = '\0';
  long double value = strtold(buf, &eptr);
  if (*eptr != '\0' || isnan(value)) return 0;
  if (dp) *dp = value;
  return 1;
}

static int ld2string(char *buf, size_t len, long double value) {
  if (isinf(value)) {
    if (len < 5) return 0;
    if (value > 0) {
      memcpy(buf, "inf", 3);
      return 3;
    }
    memcpy(buf, "-inf", 4);
    return 4;
  }
  return snprintf(buf, len, "%.17Lg", value);
}

static int arOpAccumulate(long double *acc, long double v) {
  long double next = *acc + v;
  if (isnan(next) || isinf(next)) return 0;
  *acc = next;
  return 1;
}

static int classify(long double x) {
  return !!isnan(x) | !!isinf(x) << 1 | !!isfinite(x) << 2 |
         !!isnormal(x) << 3 | !!signbit(x) << 4 |
         (fpclassify(x) == FP_SUBNORMAL) << 5 | (fpclassify(x) == FP_ZERO) << 6;
}

int main(void) {
  long double values[] = {
      0.0L,          -0.0L,          1.5L,
      -2.25L,        LDBL_MAX,       -LDBL_MAX,
      LDBL_MIN,      LDBL_MIN / 4.0L, -LDBL_MIN / 1024.0L,
      HUGE_VALL,     -HUGE_VALL,     LDBL_MAX * 4.0L,
      nanl(""),      -nanl("7"),
  };
  for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++)
    printf("%zu %#x\n", i, classify(values[i]));

  const char *inputs[] = {"3.25", "nan", "-inf", "1e5000", "12x", ""};
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); i++) {
    long double d = -1.0L;
    int ok = string2ld(inputs[i], strlen(inputs[i]), &d);
    char out[64];
    int n = ok ? ld2string(out, sizeof(out), d) : 0;
    printf("%d %.*s\n", ok, n, out);
  }

  long double acc = 0.0L;
  long double addends[] = {1.0L, LDBL_MAX, LDBL_MAX, 2.0L, -HUGE_VALL};
  int accepted = 0;
  for (size_t i = 0; i < sizeof(addends) / sizeof(addends[0]); i++)
    accepted += arOpAccumulate(&acc, addends[i]);
  char out[64];
  int n = ld2string(out, sizeof(out), acc);
  printf("%d %.*s\n", accepted, n, out);
  return 0;
}
