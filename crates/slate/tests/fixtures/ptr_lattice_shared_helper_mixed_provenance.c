#include <stdio.h>
#include <stdlib.h>

// @check-fn-begin
static int touch_and_maybe_free(int *q, int do_free) {
  *q    = *q + 1;
  int v = *q;
  if (do_free) {
    free(q);
  }
  return v;
}
// @check-fn-end

int main(void) {
  int  stack_val  = 10;
  int *stack_ptr  = &stack_val;
  int  from_stack = touch_and_maybe_free(stack_ptr, 0);

  int *heap_ptr = malloc(sizeof(int));
  *heap_ptr     = 100;
  int from_heap = touch_and_maybe_free(heap_ptr, 1);

  printf("%d %d %d\n", stack_val, from_stack, from_heap);
  return stack_val + from_stack + from_heap;
}

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn touch_and_maybe_free(mut q: *mut i32, mut do_free: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *q = (unsafe { *q }) + (1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut v: i32 = unsafe { *q };
// CHECK-NEXT:     if do_free != (0 as i32) {
// CHECK-NEXT:         unsafe { free(q as *mut ()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     v
// CHECK: {{^}}}
// SLATE-FILECHECK-END
