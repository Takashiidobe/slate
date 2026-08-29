#include <stdio.h>
#include <stdlib.h>

static int touch_and_maybe_free(int *q, int do_free) {
  *q = *q + 1;
  int v = *q;
  if (do_free) {
    free(q);
  }
  return v;
}

int main(void) {
  int stack_val = 10;
  int *stack_ptr = &stack_val;
  int from_stack = touch_and_maybe_free(stack_ptr, 0);

  int *heap_ptr = malloc(sizeof(int));
  *heap_ptr = 100;
  int from_heap = touch_and_maybe_free(heap_ptr, 1);

  printf("%d %d %d\n", stack_val, from_stack, from_heap);
  return stack_val + from_stack + from_heap;
}
// REWRITES-DAG: fn touch_and_maybe_free(arg0: *mut i32, arg1: i32) -> i32
// REWRITES-DAG: free(
// REWRITES-NOT: fn touch_and_maybe_free({{.*}}Box
// REWRITES-NOT: Box::from_raw
