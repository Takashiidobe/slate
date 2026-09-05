#include <stdio.h>
#include <string.h>

static int get_count(void) { return 4; }

// @lowering-fn-begin
// @rewrite-fn-begin
int main(void) {
  unsigned char equal_a[4]   = {1, 2, 3, 4};
  unsigned char equal_b[4]   = {1, 2, 3, 4};
  int           equal_result = memcmp(equal_a, equal_b, 4) == 0;

  unsigned char unequal_a[4]   = {1, 2, 3, 4};
  unsigned char unequal_b[4]   = {1, 2, 3, 9};
  int           unequal_result = memcmp(unequal_a, unequal_b, 4) == 0;

  unsigned char partial_a[8]   = {1, 2, 3, 4, 9, 9, 9, 9};
  unsigned char partial_b[8]   = {1, 2, 3, 4, 0, 0, 0, 0};
  int           partial_result = memcmp(partial_a, partial_b, 4) == 0;

  unsigned char dyn_a[8]   = {1, 2, 3, 4, 5, 6, 7, 8};
  unsigned char dyn_b[8]   = {1, 2, 3, 4, 5, 6, 7, 8};
  int           n          = get_count();
  int           dyn_result = memcmp(dyn_a, dyn_b, n) == 0;

  printf("%d %d %d %d\n", equal_result, unequal_result, partial_result,
         dyn_result);
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn main() {
// COMMON-LOWERING-DAG:     let mut equal_a: [u8; 4] = [0; 4];
// COMMON-LOWERING-DAG:     let mut equal_b: [u8; 4] = [0; 4];
// COMMON-LOWERING-DAG:     let mut unequal_a: [u8; 4] = [0; 4];
// COMMON-LOWERING-DAG:     let mut unequal_b: [u8; 4] = [0; 4];
// COMMON-LOWERING-DAG:     let mut partial_a: [u8; 8] = [0; 8];
// COMMON-LOWERING-DAG:     let mut partial_b: [u8; 8] = [0; 8];
// COMMON-LOWERING-DAG:     let mut dyn_a: [u8; 8] = [0; 8];
// COMMON-LOWERING-DAG:     let mut dyn_b: [u8; 8] = [0; 8];
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 4] = [1, 2, 3, 4];
// COMMON-LOWERING-DAG:     equal_a = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 4] = [1, 2, 3, 4];
// COMMON-LOWERING-DAG:     equal_b = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = equal_a.as_mut_ptr() as *mut u8;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = equal_b.as_mut_ptr() as *mut u8;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-DAG:         memcmp(
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-DAG:         )
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 4] = [1, 2, 3, 4];
// COMMON-LOWERING-DAG:     unequal_a = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 4] = [1, 2, 3, 9];
// COMMON-LOWERING-DAG:     unequal_b = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = unequal_a.as_mut_ptr() as *mut u8;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = unequal_b.as_mut_ptr() as *mut u8;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-DAG:         memcmp(
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-DAG:         )
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 8] = [1, 2, 3, 4, 9, 9, 9, 9];
// COMMON-LOWERING-DAG:     partial_a = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 8] = [1, 2, 3, 4, 0, 0, 0, 0];
// COMMON-LOWERING-DAG:     partial_b = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = partial_a.as_mut_ptr() as *mut u8;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = partial_b.as_mut_ptr() as *mut u8;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-DAG:         memcmp(
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-DAG:         )
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// COMMON-LOWERING-DAG:     dyn_a = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// COMMON-LOWERING-DAG:     dyn_b = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = get_count();
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = dyn_a.as_mut_ptr() as *mut u8;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = dyn_b.as_mut_ptr() as *mut u8;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-DAG:         memcmp(
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-DAG:         )
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-DAG:         printf(
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_char,
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
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn main() {
// COMMON-REWRITES-DAG:     let mut equal_a: [u8; 4] = [0; 4];
// COMMON-REWRITES-DAG:     let mut equal_b: [u8; 4] = [0; 4];
// COMMON-REWRITES-DAG:     let mut unequal_a: [u8; 4] = [0; 4];
// COMMON-REWRITES-DAG:     let mut unequal_b: [u8; 4] = [0; 4];
// COMMON-REWRITES-DAG:     let mut partial_a: [u8; 8] = [0; 8];
// COMMON-REWRITES-DAG:     let mut partial_b: [u8; 8] = [0; 8];
// COMMON-REWRITES-DAG:     let mut dyn_a: [u8; 8] = [0; 8];
// COMMON-REWRITES-DAG:     let mut dyn_b: [u8; 8] = [0; 8];
// COMMON-REWRITES-DAG:     equal_a = [1, 2, 3, 4];
// COMMON-REWRITES-DAG:     equal_b = [1, 2, 3, 4];
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = equal_a.as_mut_ptr() as *mut u8;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = equal_b.as_mut_ptr() as *mut u8;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-DAG:         memcmp(
// COMMON-REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-REWRITES-DAG:             (4 as u64) as usize,
// COMMON-REWRITES-DAG:         )
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} == 0) as i32;
// COMMON-REWRITES-DAG:     unequal_a = [1, 2, 3, 4];
// COMMON-REWRITES-DAG:     unequal_b = [1, 2, 3, 9];
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = unequal_a.as_mut_ptr() as *mut u8;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = unequal_b.as_mut_ptr() as *mut u8;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-DAG:         memcmp(
// COMMON-REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-REWRITES-DAG:             (4 as u64) as usize,
// COMMON-REWRITES-DAG:         )
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} == 0) as i32;
// COMMON-REWRITES-DAG:     partial_a = [1, 2, 3, 4, 9, 9, 9, 9];
// COMMON-REWRITES-DAG:     partial_b = [1, 2, 3, 4, 0, 0, 0, 0];
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = partial_a.as_mut_ptr() as *mut u8;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = partial_b.as_mut_ptr() as *mut u8;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-DAG:         memcmp(
// COMMON-REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-REWRITES-DAG:             (4 as u64) as usize,
// COMMON-REWRITES-DAG:         )
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} == 0) as i32;
// COMMON-REWRITES-DAG:     dyn_a = [1, 2, 3, 4, 5, 6, 7, 8];
// COMMON-REWRITES-DAG:     dyn_b = [1, 2, 3, 4, 5, 6, 7, 8];
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = get_count();
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = dyn_a.as_mut_ptr() as *mut u8;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = dyn_b.as_mut_ptr() as *mut u8;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-DAG:         memcmp(
// COMMON-REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-REWRITES-DAG:             ({{__v[0-9]+}} as u64) as usize,
// COMMON-REWRITES-DAG:         )
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     unsafe {
// COMMON-REWRITES-DAG:         printf(
// COMMON-REWRITES-DAG:             c"%d %d %d %d\n".as_ptr(),
// COMMON-REWRITES-DAG:             {{__v[0-9]+}},
// COMMON-REWRITES-DAG:             {{__v[0-9]+}},
// COMMON-REWRITES-DAG:             {{__v[0-9]+}},
// COMMON-REWRITES-DAG:             ({{__v[0-9]+}} == 0) as i32,
// COMMON-REWRITES-DAG:         )
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     std::process::exit(0 as i32);
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
