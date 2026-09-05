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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn main() {
// COMMON-LOWERING-DAG:     let mut a: [i32; 3] = [0; 3];
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: [i32; 4] = [10, 20, 30, 40];
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut i32 = arr.as_mut_ptr() as *mut i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = sum_fixed({{__v[0-9]+}});
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: [i32; 3] = [1, 2, 3];
// COMMON-LOWERING-DAG:     a = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut i32 = a.as_mut_ptr() as *mut i32;
// COMMON-LOWERING-DAG:     scale_fixed({{__v[0-9]+}});
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 3;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{__v[0-9]+}} as usize) };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut i32 = {{__v[0-9]+}} as *mut i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(0) };
// COMMON-LOWERING-DAG:     unsafe {
// COMMON-LOWERING-DAG:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 6;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-LOWERING-DAG:     unsafe {
// COMMON-LOWERING-DAG:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(2) };
// COMMON-LOWERING-DAG:     unsafe {
// COMMON-LOWERING-DAG:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = sum3({{__v[0-9]+}});
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-DAG:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: [i32; 6] = [1, 2, 3, 4, 5, 6];
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut i32 = big.as_mut_ptr() as *mut i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = mix({{__v[0-9]+}});
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = a[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = a[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-DAG:         printf(
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-DAG:             {{__v[0-9]+}},
// COMMON-LOWERING-DAG:             {{__v[0-9]+}},
// COMMON-LOWERING-DAG:             {{__v[0-9]+}},
// COMMON-LOWERING-DAG:             {{__v[0-9]+}},
// COMMON-LOWERING-DAG:             {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         )
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG:     let mut arr: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-X86_64-GNU-DAG:     let mut big: aligned::Aligned<aligned::A16, [i32; 6]> = aligned::Aligned([0; 6]);
// LOWERING-X86_64-GNU-DAG:     *arr = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     *big = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG:     let mut arr: [i32; 4] = [0; 4];
// LOWERING-AARCH64-GNU-DAG:     let mut big: [i32; 6] = [0; 6];
// LOWERING-AARCH64-GNU-DAG:     arr = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:     big = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn main() {
// COMMON-REWRITES-DAG:     let mut a: [i32; 3] = [0; 3];
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = arr.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = sum_fixed(unsafe { std::slice::from_raw_parts({{__v[0-9]+}} as *const i32, 4usize) });
// COMMON-REWRITES-DAG:     a = [1, 2, 3];
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = a.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-DAG:     scale_fixed(unsafe { std::slice::from_raw_parts_mut({{__v[0-9]+}} as *mut i32, 3usize) });
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: u64 = 3 * {{__v[0-9]+}};
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{__v[0-9]+}} as usize) };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = {{__v[0-9]+}} as *mut i32;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(0) };
// COMMON-REWRITES-DAG:     unsafe {
// COMMON-REWRITES-DAG:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 6;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-REWRITES-DAG:     unsafe {
// COMMON-REWRITES-DAG:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(2) };
// COMMON-REWRITES-DAG:     unsafe {
// COMMON-REWRITES-DAG:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = sum3(unsafe { std::slice::from_raw_parts({{__v[0-9]+}} as *const i32, 3usize) });
// COMMON-REWRITES-DAG:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = big.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-DAG:     unsafe {
// COMMON-REWRITES-DAG:         printf(
// COMMON-REWRITES-DAG:             c"%d %d %d %d %d\n".as_ptr(),
// COMMON-REWRITES-DAG:             {{__v[0-9]+}},
// COMMON-REWRITES-DAG:             a[0],
// COMMON-REWRITES-DAG:             a[2],
// COMMON-REWRITES-DAG:             {{__v[0-9]+}},
// COMMON-REWRITES-DAG:             mix({{__v[0-9]+}}),
// COMMON-REWRITES-DAG:         )
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     std::process::exit(0 as i32);
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG:     let mut arr: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-X86_64-GNU-DAG:     let mut big: aligned::Aligned<aligned::A16, [i32; 6]> = aligned::Aligned([0; 6]);
// REWRITES-X86_64-GNU-DAG:     *arr = [10, 20, 30, 40];
// REWRITES-X86_64-GNU-DAG:     *big = [1, 2, 3, 4, 5, 6];
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG:     let mut arr: [i32; 4] = [0; 4];
// REWRITES-AARCH64-GNU-DAG:     let mut big: [i32; 6] = [0; 6];
// REWRITES-AARCH64-GNU-DAG:     arr = [10, 20, 30, 40];
// REWRITES-AARCH64-GNU-DAG:     big = [1, 2, 3, 4, 5, 6];
// SLATE-FILECHECK-END rewrites-aarch64-gnu
