void abort(void);

int sub1(int i, int j) {
  typedef int c[i + 2];
  int         x[10], y[10];

  if (j == 2) {
    __builtin_memcpy(x, y, 10 * sizeof(int));
    return sizeof(c);
  } else
    return sizeof(c) * 3;
}

int main() {
  if (sub1(20, 3) != 66 * sizeof(int))
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     #[link_name = "memcpy"]
// CHECK-NEXT:     fn __builtin_memcpy([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn sub1(mut i: i32, mut j: i32) -> i32 {
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u64 = ((i + (2 as i32)) as i64) as u64;
// CHECK-NEXT:     let mut x: __SlateAlign16<[i32; 10]> = __SlateAlign16([0 as i32; 10]);
// CHECK-NEXT:     let mut y: __SlateAlign16<[i32; 10]> = __SlateAlign16([0 as i32; 10]);
// CHECK-NEXT:     if j == (2 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __builtin_memcpy(
// CHECK-NEXT:                 x.0.as_mut_ptr() as *mut (),
// CHECK-NEXT:                 y.0.as_mut_ptr() as *const (),
// CHECK-NEXT:                 (((10 as i32) as i64) as u64).wrapping_mul(4 as u64),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         return ([[V_3]].wrapping_mul(4 as u64) as u32) as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return ([[V_3]].wrapping_mul(4 as u64).wrapping_mul(((3 as i32) as i64) as u64)
// CHECK-NEXT:             as u32) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if ((sub1(20 as i32, 3 as i32) as i64) as u64)
// CHECK-NEXT:         != (((66 as i32) as i64) as u64).wrapping_mul(4 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
