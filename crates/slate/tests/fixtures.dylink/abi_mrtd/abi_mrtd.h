#ifndef ABI_MRTD_H
#define ABI_MRTD_H

struct pair {
  int a;
  int b;
};

typedef int (*binary_fn)(int, int);
typedef long long (*wide_fn)(long long, int, short);
typedef int (*apply_fn)(binary_fn, int, int);
typedef int (*sum_fn)(int, ...);
typedef struct pair (*make_fn)(int, int);
typedef double (*mix_fn)(float, double, int);

#endif
