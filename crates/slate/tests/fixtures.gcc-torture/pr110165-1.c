struct s {
  int t : 1;
};

int f(struct s t, int a, int b) __attribute__((noinline));
int f(struct s t, int a, int b) {
  int bd = t.t;
  if (bd)
    a |= b;
  return a;
}

int main(void) {
  struct s t;
  for (int i = -1; i <= 1; i++) {
    int      a   = 0x10;
    int      b   = 0x0f;
    int      c   = a | b;
    struct s t   = {i};
    int      r   = f(t, a, b);
    int      exp = (i != 0) ? a | b : a;
    if (exp != r)
      __builtin_abort();
  }
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct s {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u8,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits0U0 {
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub t: i32,
// CHECK-NEXT:         #[bits(7, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f(mut t: s, mut a: i32, mut b: i32) -> i32 {
// CHECK-NEXT:     let mut bd: i32 = t.__slate_bits_0.__get_t();
// CHECK-NEXT:     if bd != (0 as i32) {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = a;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] | b;
// CHECK-NEXT:         a = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     a
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut t: s = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: i32 = -(1 as i32);
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i <= (1 as i32) {
// CHECK-NEXT:         let mut a: i32 = 16 as i32;
// CHECK-NEXT:         let mut b: i32 = 15 as i32;
// CHECK-NEXT:         let mut t: s = {
// CHECK-NEXT:             let mut [[T_0:__t[0-9]+]]: s = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_0]].__slate_bits_0.__set_t(i);
// CHECK-NEXT:             [[T_0]]
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut r: i32 = f(t, a, b);
// CHECK-NEXT:         let mut exp: i32 = if i != (0 as i32) { a | b } else { a };
// CHECK-NEXT:         if exp != r {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     0
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
