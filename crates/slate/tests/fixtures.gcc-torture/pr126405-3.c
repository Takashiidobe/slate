/* A parameter that stays live across a redefinition of itself needs two
   partitions, and an oversized vector parameter (V16DI, 128 bytes, no register
   mode) puts both of them in memory.  Every name of both partitions has the
   PARM_DECL as its base, so without a split the incoming argument slot and the
   local slot both claim to be the parameter, and the load/store pair-fusion
   pass fuses accesses across them.  Self-checking: aborts if the result is
   wrong.  */

typedef long __attribute__((vector_size(16 * sizeof(long)))) v16di;

v16di g0, g1;

/* The wrong value is read from an uninitialised stack slot, so make sure the
   stack the callee reuses does not happen to be zero.  */
__attribute__((noipa)) static void dirty_stack(void) {
  volatile char buf[1024];
  for (unsigned i = 0; i < sizeof(buf); i++)
    buf[i] = 0xa5;
}

__attribute__((noipa)) static v16di f(v16di p) {
  v16di old = p;
  p         = g0;
  g1        = p;
  return old + p;
}

int main(void) {
  v16di a, r;

  for (int i = 0; i < 16; i++) {
    a[i]  = i + 1;
    g0[i] = 100;
  }
  dirty_stack();
  r = f(a);
  for (int i = 0; i < 16; i++)
    if (r[i] != i + 101 || g1[i] != 100)
      __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! A parameter that stays live across a redefinition of itself needs two
// CHECK-NEXT: //! partitions, and an oversized vector parameter (V16DI, 128 bytes, no register
// CHECK-NEXT: //! mode) puts both of them in memory.  Every name of both partitions has the
// CHECK-NEXT: //! PARM_DECL as its base, so without a split the incoming argument slot and the
// CHECK-NEXT: //! local slot both claim to be the parameter, and the load/store pair-fusion
// CHECK-NEXT: //! pass fuses accesses across them.  Self-checking: aborts if the result is
// CHECK-NEXT: //! wrong.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut g0: std::simd::Simd<i64, 16> = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut g1: std::simd::Simd<i64, 16> = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: /// The wrong value is read from an uninitialised stack slot, so make sure the
// CHECK-NEXT: /// stack the callee reuses does not happen to be zero.
// CHECK-NEXT: fn dirty_stack() {
// CHECK-NEXT:     let mut buf: __SlateAlign16<[i8; 1024]> = __SlateAlign16([0 as i8; 1024]);
// CHECK-NEXT:     let mut i: u32 = (0 as i32) as u32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (i as u64) < (1024 as u64) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 unsafe { (buf.0.as_mut_ptr() as *mut i8).offset(i as isize) },
// CHECK-NEXT:                 (165 as i32) as i8,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u32 = [[V_0]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut p: std::simd::Simd<i64, 16>) -> std::simd::Simd<i64, 16> {
// CHECK-NEXT:     let mut old: std::simd::Simd<i64, 16> = p;
// CHECK-NEXT:     p = unsafe { g0 };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g1 = p;
// CHECK-NEXT:     }
// CHECK-NEXT:     return old + p;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: std::simd::Simd<i64, 16> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut r: std::simd::Simd<i64, 16> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (16 as i32) {
// CHECK-NEXT:             a[(i as usize)] = (i + (1 as i32)) as i64;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 g0[(i as usize)] = (100 as i32) as i64;
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:             i = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     dirty_stack();
// CHECK-NEXT:     r = f(a);
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while i < (16 as i32) {
// CHECK-NEXT:         if r[(i as usize)] != ((i + (101 as i32)) as i64)
// CHECK-NEXT:             || (unsafe { g1[(i as usize)] }) != ((100 as i32) as i64)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:         i = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
