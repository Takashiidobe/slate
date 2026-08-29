#include <stdio.h>

static int read_pointer(int *pointer) { return *pointer; }

static int read_array_element_through_call(void) {
  int  values[1] = {10};
  int *pointer   = &values[0];
  return read_pointer(pointer);
}

int main(void) {
  int  values[4] = {2, 4, 6, 8};
  int *p         = &values[1];
  int *q         = &values[3];
  printf("%d %ld %d\n", *p + *q, q - p, read_array_element_through_call());
  return 0;
}
// REWRITES-NOT: unsafe { *p }
// REWRITES-NOT: unsafe { *q }
// REWRITES-NOT: unsafe { *pointer }
