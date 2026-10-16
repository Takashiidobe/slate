/* { dg-require-effective-target int32plus } */

/* Failed on sparc with -mv8plus because sparc.c:set_extends() thought
   erroneously that SImode ASHIFT chops the upper bits, it does not.  */

typedef unsigned long long uint64;

void g(uint64 x, int y, int z, uint64 *p) {
  unsigned w  = ((x >> y) & 0xffffffffULL) << (z & 0x1f);
  *p         |= (w & 0xffffffffULL) << z;
}

int main(void) {
  uint64 a = 0;
  g(0xdeadbeef01234567ULL, 0, 0, &a);
  return (a == 0x01234567) ? 0 : 1;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-require-effective-target int32plus }
// CHECK-NEXT: // Failed on sparc with -mv8plus because sparc.c:set_extends() thought
// CHECK-NEXT: // erroneously that SImode ASHIFT chops the upper bits, it does not.
// CHECK-NEXT: fn g(mut x: u64, mut y: i32, mut z: i32, mut p: &mut u64) {
// CHECK-NEXT:     let mut w: u32 = ((x >> y & (4294967295 as u64)) << (z & (31 as i32))) as u32;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u64 = unsafe { *p };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u64 = [[V_0]] | ((w as u64) & (4294967295 as u64)) << z;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *p = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: u64 = ((0 as i32) as i64) as u64;
// CHECK-NEXT:     g(16045690981116495207 as u64, 0 as i32, 0 as i32, unsafe {
// CHECK-NEXT:         &mut (*std::ptr::addr_of_mut!(a))
// CHECK-NEXT:     });
// CHECK-NEXT:     if a == (((19088743 as i32) as i64) as u64) {
// CHECK-NEXT:         0 as i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         1 as i32
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
