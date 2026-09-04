#include <stdio.h>
#include <stdlib.h>

static int sum_fixed(int *p) {
  int total = 0;
  for (int i = 0; i < 4; i++) {
    total += p[i];
  }
  return total;
}

static void scale_fixed(int *p) {
  for (int i = 0; i < 3; i++) {
    p[i] = p[i] * 2;
  }
}

static int sum3(int *p) {
  int t = 0;
  for (int i = 0; i < 3; i++) {
    t += p[i];
  }
  return t;
}

static int mix(int *p) {
  int t = 0;
  for (int i = 0; i < 3; i++) {
    t += p[i];
  }
  t += p[5];
  return t;
}

// @lowering-fn-begin
// @rewrite-fn-begin
int main(void) {
  int arr[4] = {10, 20, 30, 40};
  int r      = sum_fixed(arr);

  int a[3] = {1, 2, 3};
  scale_fixed(a);

  int *m = malloc(3 * sizeof(int));
  m[0]   = 5;
  m[1]   = 6;
  m[2]   = 7;
  int r2 = sum3(m);
  free(m);

  int big[6] = {1, 2, 3, 4, 5, 6};
  int r3     = mix(big);

  printf("%d %d %d %d %d\n", r, a[0], a[2], r2, r3);
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let mut arr: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-DAG:     let mut a: [i32; 3] = [0; 3];
// LOWERING-DAG:     let mut big: aligned::Aligned<aligned::A16, [i32; 6]> = aligned::Aligned([0; 6]);
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     *arr = [10, 20, 30, 40];
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut i32 = arr.as_mut_ptr() as *mut i32;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = sum_fixed({{_v[0-9]+}});
// LOWERING-DAG:     a = [1, 2, 3];
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut i32 = a.as_mut_ptr() as *mut i32;
// LOWERING-DAG:     scale_fixed({{_v[0-9]+}});
// LOWERING-DAG:     let {{_v[0-9]+}}: u64 = 3;
// LOWERING-DAG:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-DAG:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{_v[0-9]+}} as usize) };
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut i32 = {{_v[0-9]+}} as *mut i32;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-DAG:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(0) };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 6;
// LOWERING-DAG:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(1) };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 7;
// LOWERING-DAG:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(2) };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = sum3({{_v[0-9]+}});
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     unsafe { free({{_v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-DAG:     *big = [1, 2, 3, 4, 5, 6];
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut i32 = big.as_mut_ptr() as *mut i32;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = mix({{_v[0-9]+}});
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-DAG:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = a[({{_v[0-9]+}} as usize)];
// LOWERING-DAG:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = a[({{_v[0-9]+}} as usize)];
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-DAG:         printf(
// LOWERING-DAG:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-DAG:             {{_v[0-9]+}},
// LOWERING-DAG:             {{_v[0-9]+}},
// LOWERING-DAG:             {{_v[0-9]+}},
// LOWERING-DAG:             {{_v[0-9]+}},
// LOWERING-DAG:             {{_v[0-9]+}},
// LOWERING-DAG:         )
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let mut arr: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-DAG:     let mut a: [i32; 3] = [0; 3];
// REWRITES-DAG:     let mut big: aligned::Aligned<aligned::A16, [i32; 6]> = aligned::Aligned([0; 6]);
// REWRITES-DAG:     *arr = [10, 20, 30, 40];
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut i32 = arr.as_mut_ptr() as *mut i32;
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = sum_fixed(unsafe { std::slice::from_raw_parts({{_v[0-9]+}} as *const i32, 4usize) });
// REWRITES-DAG:     a = [1, 2, 3];
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut i32 = a.as_mut_ptr() as *mut i32;
// REWRITES-DAG:     scale_fixed(unsafe { std::slice::from_raw_parts_mut({{_v[0-9]+}} as *mut i32, 3usize) });
// REWRITES-DAG:     let {{_v[0-9]+}}: u64 = 4;
// REWRITES-DAG:     let {{_v[0-9]+}}: u64 = 3 * {{_v[0-9]+}};
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{_v[0-9]+}} as usize) };
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut i32 = {{_v[0-9]+}} as *mut i32;
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = 5;
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(0) };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = 6;
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(1) };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = 7;
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(2) };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = sum3(unsafe { std::slice::from_raw_parts({{_v[0-9]+}} as *const i32, 3usize) });
// REWRITES-DAG:     unsafe { free({{_v[0-9]+}} as *mut core::ffi::c_void) };
// REWRITES-DAG:     *big = [1, 2, 3, 4, 5, 6];
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut i32 = big.as_mut_ptr() as *mut i32;
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         printf(
// REWRITES-DAG:             c"%d %d %d %d %d\n".as_ptr(),
// REWRITES-DAG:             {{_v[0-9]+}},
// REWRITES-DAG:             a[0],
// REWRITES-DAG:             a[2],
// REWRITES-DAG:             {{_v[0-9]+}},
// REWRITES-DAG:             mix({{_v[0-9]+}}),
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
