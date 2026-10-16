/* AArch64 wrong code at -O2.  Store motion creates several SSA versions of an
   oversized vector temporary (V16DI, 128 bytes, no register mode).  The
   partition holding the loop-carried versions has an anonymous representative,
   so out-of-SSA left it and the partition of the copy taken for the use after
   the loop sharing one MEM_EXPR.  The load/store pair-fusion pass then treated
   two stores 144 bytes apart as adjacent, fused them, and left the tail of one
   slot uninitialised.  Self-checking: aborts if the result is wrong.  */

typedef long __attribute__((vector_size(16 * sizeof(long)))) v16di;
typedef int __attribute__((vector_size(16 * sizeof(int))))   v16si;

long  g2, g12;
v16di g18;
v16si g3;
void *g27;

/* The wrong value is read from an uninitialised stack slot, so make sure the
   stack the callee reuses does not happen to be zero.  */
__attribute__((noipa)) static void dirty_stack(void) {
  volatile char buf[1024];
  for (unsigned i = 0; i < sizeof(buf); i++)
    buf[i] = 0xa5;
}

void f31(void) {
lbl_br1:
  g18 = ~g18;
  g3  = ~g3;
  if (g2)
    goto lbl_br1;
lbl_b5:
  switch (g12)
  case 4:
  case 0:
    goto lbl_sw8;
  __builtin_abort();
lbl_sw8:
  if (g27)
    goto lbl_b5;
  g18 = ~g18;
}

int main(void) {
  dirty_stack();
  f31();
  for (int i = 0; i < 16; i++)
    if (g18[i] != 0)
      __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! AArch64 wrong code at -O2.  Store motion creates several SSA versions of an
// CHECK-NEXT: //! oversized vector temporary (V16DI, 128 bytes, no register mode).  The
// CHECK-NEXT: //! partition holding the loop-carried versions has an anonymous representative,
// CHECK-NEXT: //! so out-of-SSA left it and the partition of the copy taken for the use after
// CHECK-NEXT: //! the loop sharing one MEM_EXPR.  The load/store pair-fusion pass then treated
// CHECK-NEXT: //! two stores 144 bytes apart as adjacent, fused them, and left the tail of one
// CHECK-NEXT: //! slot uninitialised.  Self-checking: aborts if the result is wrong.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut g2: i64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut g12: i64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut g18: std::simd::Simd<i64, 16> = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut g3: std::simd::Simd<i32, 16> = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut g27: *mut () = unsafe { std::mem::zeroed() };
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
// CHECK-NEXT: fn f31() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 g18 = !unsafe { g18 };
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 g3 = !unsafe { g3 };
// CHECK-NEXT:             }
// CHECK-NEXT:             if !((unsafe { g2 }) != (0 as i64)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         loop {
// CHECK-NEXT:             let [[T_0:__t[0-9]+]]: i64 = unsafe { g12 };
// CHECK-NEXT:             if [[T_0]] == (4 as i64) {
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 if [[T_0]] == (0 as i64) {
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     unsafe { __builtin_abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             if !((unsafe { g27 }) != std::ptr::null_mut::<()>()) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             g18 = !unsafe { g18 };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     dirty_stack();
// CHECK-NEXT:     f31();
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (16 as i32) {
// CHECK-NEXT:         if (unsafe { g18[(i as usize)] }) != ((0 as i32) as i64) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
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
