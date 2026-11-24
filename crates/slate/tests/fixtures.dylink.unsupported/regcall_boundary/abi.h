#ifndef REGCALL_BOUNDARY_H
#define REGCALL_BOUNDARY_H

typedef long (__attribute__((regcall)) *sum_fn)(long, long, long, long, long, long);
typedef int (*check_fn)(sum_fn);

#endif
