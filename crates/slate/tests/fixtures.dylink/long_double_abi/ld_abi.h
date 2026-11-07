#ifndef LD_ABI_H
#define LD_ABI_H

#include <stddef.h>
#include <stdint.h>

struct ld_pair {
  long double a;
  long double b;
};

struct ld_one {
  long double v;
};

struct ld_box {
  unsigned char tag;
  long double x;
  uint32_t tail;
};

typedef long double (*ld_binary_fn)(long double, long double);

#endif
