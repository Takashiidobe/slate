#include <stdio.h>

static int parameter_bound(int length, int values[length++]) {
  return length + values[0];
}

// @lowering-fn-begin
// @rewrite-fn-begin
int main(void) {
  int           bound       = 3;
  unsigned long evaluated   = sizeof(int[bound++]);
  unsigned long unevaluated = sizeof(int (*)[bound++]);
  int           values[]    = {7, 8, 9};
  int           parameter   = parameter_bound(3, values);
  printf("%lu %lu %d %d\n", evaluated, unevaluated, bound, parameter);
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn main() {
// COMMON-LOWERING-DAG:     let mut values: [i32; 3] = [0; 3];
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 8;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: [i32; 3] = [7, 8, 9];
// COMMON-LOWERING-DAG:     values = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = parameter_bound({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"%lu %lu %d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"%lu %lu %d %d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn main() {
// COMMON-REWRITES-DAG:     let mut values: [i32; 3] = [0; 3];
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: u64 = 4 * ({{__v[0-9]+}} as u64);
// COMMON-REWRITES-DAG:     values = [7, 8, 9];
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-DAG:     unsafe {
// COMMON-REWRITES-DAG:         printf(
// COMMON-REWRITES-DAG:             c"%lu %lu %d %d\n".as_ptr(),
// COMMON-REWRITES-DAG:             {{__v[0-9]+}},
// COMMON-REWRITES-DAG:             8 as u64,
// COMMON-REWRITES-DAG:             {{__v[0-9]+}},
// COMMON-REWRITES-DAG:             parameter_bound(3, {{__v[0-9]+}}),
// COMMON-REWRITES-DAG:         )
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     std::process::exit(0 as i32);
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
