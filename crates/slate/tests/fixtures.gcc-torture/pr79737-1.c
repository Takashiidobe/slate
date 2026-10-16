/* PR tree-optimization/79737 */

#if __SIZEOF_INT__ < 4
__extension__ typedef __INT32_TYPE__ int32_t;
#else
typedef int int32_t;
#endif

#pragma pack(1)
struct S {
  int32_t b : 18;
  int32_t c : 1;
  int32_t d : 24;
  int32_t e : 15;
  int32_t f : 14;
} i;
int             g, j, k;
static struct S h;

void foo() {
  for (j = 0; j < 6; j++)
    k = 0;
  for (; k < 3; k++) {
    struct S m = {5, 0, -5, 9, 5};
    h          = m;
    if (g)
      i = m;
    h.e = 0;
  }
}

int main() {
  foo();
  if (h.e != 0)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/79737
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits1U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield([u8; 9], c_names = true, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// CHECK-NEXT:     pub struct __SlateBits1U0 {
// CHECK-NEXT:         #[bits(18)]
// CHECK-NEXT:         pub b: i32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub c: i32,
// CHECK-NEXT:         #[bits(24)]
// CHECK-NEXT:         pub d: i32,
// CHECK-NEXT:         #[bits(15)]
// CHECK-NEXT:         pub e: i32,
// CHECK-NEXT:         #[bits(14)]
// CHECK-NEXT:         pub f: i32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut i: S = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut j: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut k: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut h: S = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         j = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { j }) < (6 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             k = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = unsafe { j };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             j = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while (unsafe { k }) < (3 as i32) {
// CHECK-NEXT:         let mut m: S = {
// CHECK-NEXT:             let mut [[T_0:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_0]].__slate_bits_0.__set_b(5 as i32);
// CHECK-NEXT:             [[T_0]].__slate_bits_0.__set_c(0 as i32);
// CHECK-NEXT:             [[T_0]].__slate_bits_0.__set_d(-(5 as i32));
// CHECK-NEXT:             [[T_0]].__slate_bits_0.__set_e(9 as i32);
// CHECK-NEXT:             [[T_0]].__slate_bits_0.__set_f(5 as i32);
// CHECK-NEXT:             [[T_0]]
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             h = m;
// CHECK-NEXT:         }
// CHECK-NEXT:         if (unsafe { g }) != (0 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 i = m;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*std::ptr::addr_of_mut!(h.__slate_bits_0)).__set_e(0 as i32);
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = unsafe { k };
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             k = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo();
// CHECK-NEXT:     if (unsafe { (*std::ptr::addr_of!(h.__slate_bits_0)).__get_e() }) != (0 as i32) {
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
