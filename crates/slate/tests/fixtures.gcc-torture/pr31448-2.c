/* PR middle-end/31448, this used to ICE during expand because
   reduce_to_bit_field_precision was not ready to handle constants. */

typedef struct _st {
  long int iIndex  : 24;
  long int iIndex1 : 24;
} st;
st  *next;
void g(void) {
  st              *next = 0;
  int              nIndx;
  const static int constreg[] = {
      0,
  };
  nIndx        = 0;
  next->iIndex = constreg[nIndx];
}
void f(void) {
  int                   nIndx;
  const static long int constreg[] = {
      0xFEFEFEFE,
  };
  nIndx         = 0;
  next->iIndex  = constreg[nIndx];
  next->iIndex1 = constreg[nIndx];
}
int main(void) {
  st a;
  next = &a;
  f();
  if (next->iIndex != 0xFFFEFEFE)
    __builtin_abort();
  if (next->iIndex1 != 0xFFFEFEFE)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/31448, this used to ICE during expand because
// CHECK-NEXT: //! reduce_to_bit_field_precision was not ready to handle constants.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(8))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _st {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield([u8; 6], c_names = true, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// CHECK-NEXT:     pub struct __SlateBits0U0 {
// CHECK-NEXT:         #[bits(24)]
// CHECK-NEXT:         pub iIndex: i64,
// CHECK-NEXT:         #[bits(24)]
// CHECK-NEXT:         pub iIndex1: i64,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut next: *mut _st = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn g() {
// CHECK-NEXT:     let mut [[NEXT_0:next_[0-9]+]]: *mut _st = std::ptr::null_mut::<_st>();
// CHECK-NEXT:     let mut nIndx: i32 = 0 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*[[NEXT_0]]).__slate_bits_0.__set_iIndex(
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of!([[CONSTREG_0:constreg_[0-9]+]]) as *const i32 }.offset(nIndx as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             }) as i64,
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut [[CONSTREG_0]]: [i32; 1] = [0 as i32];
// CHECK-EMPTY:
// CHECK-NEXT: fn f() {
// CHECK-NEXT:     let mut nIndx: i32 = 0 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { next }).__slate_bits_0.__set_iIndex(unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of!([[CONSTREG_1:constreg_[0-9]+]]) as *const i64 }.offset(nIndx as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         });
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { next }).__slate_bits_0.__set_iIndex1(unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of!([[CONSTREG_1]]) as *const i64 }.offset(nIndx as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         });
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut [[CONSTREG_1]]: [i64; 1] = [((4278124286 as u32) as u64) as i64];
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: _st = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         next = std::ptr::addr_of_mut!(a);
// CHECK-NEXT:     }
// CHECK-NEXT:     f();
// CHECK-NEXT:     if (((unsafe { (*unsafe { next }).__slate_bits_0.__get_iIndex() }) as i32) as u32)
// CHECK-NEXT:         != (4294901502 as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe { (*unsafe { next }).__slate_bits_0.__get_iIndex1() }) as i32) as u32)
// CHECK-NEXT:         != (4294901502 as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
