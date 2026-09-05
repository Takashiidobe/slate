#include <stdio.h>
#include <stdlib.h>

// @lowering-fn-begin
// @rewrite-fn-begin
static int touch_and_maybe_free(int *q, int do_free) {
  *q    = *q + 1;
  int v = *q;
  if (do_free) {
    free(q);
  }
  return v;
}
// @rewrite-fn-end
// @lowering-fn-end

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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn touch_and_maybe_free({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-DAG:     let mut q: *mut i32 = std::ptr::null_mut();
// COMMON-LOWERING-DAG:     let mut do_free: i32 = 0;
// COMMON-LOWERING-DAG:     q = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     do_free = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut i32 = q;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut i32 = q;
// COMMON-LOWERING-DAG:     unsafe {
// COMMON-LOWERING-DAG:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut i32 = q;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = do_free;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-DAG:         if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = q;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-DAG:             unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn touch_and_maybe_free(mut q: *mut i32, mut {{__v[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-DAG:     unsafe {
// COMMON-REWRITES-DAG:         *q = (unsafe { *q }) + 1;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { *q };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-DAG:     if {{__v[0-9]+}} {
// COMMON-REWRITES-DAG:         unsafe { free(q as *mut core::ffi::c_void) };
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     {{__v[0-9]+}}
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
