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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let mut equal_a: [u8; 4] = [0; 4];
// LOWERING-DAG:     let mut equal_b: [u8; 4] = [0; 4];
// LOWERING-DAG:     let mut unequal_a: [u8; 4] = [0; 4];
// LOWERING-DAG:     let mut unequal_b: [u8; 4] = [0; 4];
// LOWERING-DAG:     let mut partial_a: [u8; 8] = [0; 8];
// LOWERING-DAG:     let mut partial_b: [u8; 8] = [0; 8];
// LOWERING-DAG:     let mut dyn_a: [u8; 8] = [0; 8];
// LOWERING-DAG:     let mut dyn_b: [u8; 8] = [0; 8];
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 4] = [1, 2, 3, 4];
// LOWERING-DAG:     equal_a = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 4] = [1, 2, 3, 4];
// LOWERING-DAG:     equal_b = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = equal_a.as_mut_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = equal_b.as_mut_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 4;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-DAG:         memcmp(
// LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-DAG:             {{__v[0-9]+}} as usize,
// LOWERING-DAG:         )
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 4] = [1, 2, 3, 4];
// LOWERING-DAG:     unequal_a = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 4] = [1, 2, 3, 9];
// LOWERING-DAG:     unequal_b = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = unequal_a.as_mut_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = unequal_b.as_mut_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 4;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-DAG:         memcmp(
// LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-DAG:             {{__v[0-9]+}} as usize,
// LOWERING-DAG:         )
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 8] = [1, 2, 3, 4, 9, 9, 9, 9];
// LOWERING-DAG:     partial_a = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 8] = [1, 2, 3, 4, 0, 0, 0, 0];
// LOWERING-DAG:     partial_b = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = partial_a.as_mut_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = partial_b.as_mut_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 4;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-DAG:         memcmp(
// LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-DAG:             {{__v[0-9]+}} as usize,
// LOWERING-DAG:         )
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// LOWERING-DAG:     dyn_a = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: [u8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// LOWERING-DAG:     dyn_b = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = get_count();
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = dyn_a.as_mut_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u8 = dyn_b.as_mut_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-DAG:         memcmp(
// LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-DAG:             {{__v[0-9]+}} as usize,
// LOWERING-DAG:         )
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let mut equal_a: [u8; 4] = [0; 4];
// REWRITES-DAG:     let mut equal_b: [u8; 4] = [0; 4];
// REWRITES-DAG:     let mut unequal_a: [u8; 4] = [0; 4];
// REWRITES-DAG:     let mut unequal_b: [u8; 4] = [0; 4];
// REWRITES-DAG:     let mut partial_a: [u8; 8] = [0; 8];
// REWRITES-DAG:     let mut partial_b: [u8; 8] = [0; 8];
// REWRITES-DAG:     let mut dyn_a: [u8; 8] = [0; 8];
// REWRITES-DAG:     let mut dyn_b: [u8; 8] = [0; 8];
// REWRITES-DAG:     equal_a = [1, 2, 3, 4];
// REWRITES-DAG:     equal_b = [1, 2, 3, 4];
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = equal_a.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = equal_b.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-DAG:         memcmp(
// REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-DAG:             (4 as u64) as usize,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} == 0) as i32;
// REWRITES-DAG:     unequal_a = [1, 2, 3, 4];
// REWRITES-DAG:     unequal_b = [1, 2, 3, 9];
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = unequal_a.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = unequal_b.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-DAG:         memcmp(
// REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-DAG:             (4 as u64) as usize,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} == 0) as i32;
// REWRITES-DAG:     partial_a = [1, 2, 3, 4, 9, 9, 9, 9];
// REWRITES-DAG:     partial_b = [1, 2, 3, 4, 0, 0, 0, 0];
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = partial_a.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = partial_b.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-DAG:         memcmp(
// REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-DAG:             (4 as u64) as usize,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} == 0) as i32;
// REWRITES-DAG:     dyn_a = [1, 2, 3, 4, 5, 6, 7, 8];
// REWRITES-DAG:     dyn_b = [1, 2, 3, 4, 5, 6, 7, 8];
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = get_count();
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = dyn_a.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u8 = dyn_b.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-DAG:         memcmp(
// REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-DAG:             ({{__v[0-9]+}} as u64) as usize,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         printf(
// REWRITES-DAG:             c"%d %d %d %d\n".as_ptr(),
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             ({{__v[0-9]+}} == 0) as i32,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
