#include <stdio.h>
#include <stdlib.h>

// @lowering-fn-begin
// @rewrite-fn-begin
static int consume_extra_capacity(int *values, int len) {
  int sum = 0;
  for (int i = 0; i < len; ++i) {
    values[i]  = i + 1;
    sum       += values[i];
  }
  free(values);
  return sum;
}
// @rewrite-fn-end
// @lowering-fn-end

int main(void) {
  int  len    = 3;
  int *values = malloc((len + 1) * sizeof(int));
  int  sum    = consume_extra_capacity(values, len);
  printf("%d\n", sum);
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn consume_extra_capacity({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let mut values: *mut i32 = std::ptr::null_mut();
// LOWERING-DAG:     let mut len: i32 = 0;
// LOWERING-DAG:     let mut sum: i32 = 0;
// LOWERING-DAG:     values = {{arg[0-9]+}};
// LOWERING-DAG:     len = {{arg[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     sum = {{_v[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-DAG:         let mut i: i32 = 0;
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         i = {{_v[0-9]+}};
// LOWERING-DAG:         loop {
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = len;
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-DAG:             if !{{_v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-DAG:             {
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-DAG:                 let {{_v[0-9]+}}: *mut i32 = values;
// LOWERING-DAG:                 let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-DAG:                 unsafe {
// LOWERING-DAG:                     *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-DAG:                 }
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-DAG:                 let {{_v[0-9]+}}: *mut i32 = values;
// LOWERING-DAG:                 let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = sum;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:                 sum = {{_v[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-DAG:             i = {{_v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut i32 = values;
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     unsafe { free({{_v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = sum;
// LOWERING-DAG:     return {{_v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn consume_extra_capacity(mut values: *mut i32, mut len: i32) -> i32 {
// REWRITES-DAG:     let mut sum: i32 = 0;
// REWRITES-DAG:     for i in 0..len {
// REWRITES-DAG:         let {{_v[0-9]+}}: i32 = i + 1;
// REWRITES-DAG:         let {{_v[0-9]+}}: *mut i32 = values;
// REWRITES-DAG:         let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset((i as i64) as isize) };
// REWRITES-DAG:         unsafe {
// REWRITES-DAG:             *{{_v[0-9]+}} = {{_v[0-9]+}};
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{_v[0-9]+}}: *mut i32 = values;
// REWRITES-DAG:         let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset((i as i64) as isize) };
// REWRITES-DAG:         sum += unsafe { *{{_v[0-9]+}} };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe { free(values as *mut core::ffi::c_void) };
// REWRITES-DAG:     sum
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
