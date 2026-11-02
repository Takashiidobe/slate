// { dg-options "-fblocks" }
#include <stdio.h>

extern void *_Block_copy(const void *);
extern void _Block_release(const void *);

typedef int (^intfn)(int);

int apply_twice(intfn f, int x);
intfn make_offset(int k);
void for_each_squared(int n, void (^visit)(int));

int main(void) {
  int bias = 7;
  printf("%d\n", apply_twice(^(int x) { return x * 3 + bias; }, 2));
  intfn off = make_offset(40);
  printf("%d\n", off(2));
  _Block_release(off);
  __block long sum = 0;
  for_each_squared(5, ^(int v) { sum += v; });
  printf("%ld\n", sum);
  return 0;
}
