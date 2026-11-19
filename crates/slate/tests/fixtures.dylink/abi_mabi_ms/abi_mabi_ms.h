#ifndef ABI_MABI_MS_H
#define ABI_MABI_MS_H

#ifdef __clang__
#define MS __attribute__((ms_abi))
#else
#define MS
#endif

struct pair {
  int a;
  int b;
};

struct triple {
  long long a;
  long long b;
  long long c;
};

typedef int(MS *binary_fn)(int, int);
typedef long long(MS *wide_fn)(long long, int, short);
typedef int(MS *apply_fn)(binary_fn, int, int);
typedef int(MS *sum_fn)(int, ...);
typedef struct pair(MS *make_fn)(int, int);
typedef long long(MS *spread_fn)(struct triple, int, int, int, int);
typedef double(MS *mix_fn)(float, double, int);

#endif
