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

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-X86_64-GNU-DAG:     let mut values: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-AARCH64-GNU-DAG:     let mut values: [i32; 4] = [0; 4];
// REWRITES-DAG:     let mut c: cursor = cursor {
// REWRITES-DAG:         ptr: std::ptr::null_mut(),
// REWRITES-DAG:     };
// REWRITES-DAG:     let mut d: cursor = cursor {
// REWRITES-DAG:         ptr: std::ptr::null_mut(),
// REWRITES-DAG:     };
// REWRITES-DAG:     for i in 0..4 {
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = i;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { getchar() };
// REWRITES-DAG:         values[((i as i64) as usize)] = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-DAG:     c.ptr = {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(3) };
// REWRITES-DAG:     d.ptr = {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = d.ptr;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = c.ptr;
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         printf(
// REWRITES-DAG:             c"%d %ld\n".as_ptr(),
// REWRITES-DAG:             (unsafe { *c.ptr }) + unsafe { *d.ptr },
// REWRITES-DAG:             unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 },
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
