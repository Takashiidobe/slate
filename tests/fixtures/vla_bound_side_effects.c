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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let mut values: [i32; 3] = [0; 3];
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 4;
// LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 8;
// LOWERING-DAG:     let {{__v[0-9]+}}: [i32; 3] = [7, 8, 9];
// LOWERING-DAG:     values = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = parameter_bound({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"%lu %lu %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"%lu %lu %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let mut values: [i32; 3] = [0; 3];
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 3;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// REWRITES-DAG:     let {{__v[0-9]+}}: u64 = 4 * ({{__v[0-9]+}} as u64);
// REWRITES-DAG:     values = [7, 8, 9];
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         printf(
// REWRITES-DAG:             c"%lu %lu %d %d\n".as_ptr(),
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             8 as u64,
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             parameter_bound(3, {{__v[0-9]+}}),
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
