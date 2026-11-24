#ifndef LONG_DOUBLE_MIXED_128_H
#define LONG_DOUBLE_MIXED_128_H

typedef long double (*quad_fn)(__float128, long double);
typedef long double (*signed_fn)(__int128, long double);
typedef long double (*unsigned_fn)(unsigned __int128, long double);
typedef int (*check_quad_fn)(quad_fn);
typedef int (*check_unsigned_fn)(unsigned_fn);

#endif
