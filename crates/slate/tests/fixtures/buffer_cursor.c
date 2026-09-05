#include <stdio.h>

struct cursor {
  int *ptr;
};

// @rewrite-fn-begin
int main(void) {
  int values[4];
  for (int i = 0; i < 4; i++) {
    values[i] = i + getchar();
  }
  struct cursor c;
  c.ptr = values + 1;
  struct cursor d;
  d.ptr = values + 3;
  printf("%d %ld\n", *c.ptr + *d.ptr, d.ptr - c.ptr);
  return 0;
}
// @rewrite-fn-end

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn main() {
// COMMON-REWRITES-DAG:     let mut c: cursor = cursor {
// COMMON-REWRITES-DAG:         ptr: std::ptr::null_mut(),
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let mut d: cursor = cursor {
// COMMON-REWRITES-DAG:         ptr: std::ptr::null_mut(),
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     for i in 0..4 {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: i32 = i;
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { getchar() };
// COMMON-REWRITES-DAG:         values[((i as i64) as usize)] = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-REWRITES-DAG:     c.ptr = {{__v[0-9]+}};
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(3) };
// COMMON-REWRITES-DAG:     d.ptr = {{__v[0-9]+}};
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = d.ptr;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = c.ptr;
// COMMON-REWRITES-DAG:     unsafe {
// COMMON-REWRITES-DAG:         printf(
// COMMON-REWRITES-DAG:             c"%d %ld\n".as_ptr(),
// COMMON-REWRITES-DAG:             (unsafe { *c.ptr }) + unsafe { *d.ptr },
// COMMON-REWRITES-DAG:             unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 },
// COMMON-REWRITES-DAG:         )
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     std::process::exit(0 as i32);
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG:     let mut values: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG:     let mut values: [i32; 4] = [0; 4];
// SLATE-FILECHECK-END rewrites-aarch64-gnu
