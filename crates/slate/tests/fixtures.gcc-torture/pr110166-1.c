struct s {
  int t  : 1;
  int t1 : 1;
};

int f(struct s t) __attribute__((noinline));
int f(struct s t) {
  int c = t.t;
  int d = t.t1;
  if (c > d)
    t.t = d;
  else
    t.t = c;
  return t.t;
}

int main(void) {
  struct s t;
  for (int i = -1; i <= 0; i++) {
    for (int j = -1; j <= 0; j++) {
      struct s t   = {i, j};
      int      r   = f(t);
      int      exp = i < j ? i : j;
      if (exp != r)
        __builtin_abort();
    }
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
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub t1: i32,
// CHECK-NEXT:         #[bits(6, access = na)]
// CHECK-NEXT:         pub __slate_pad_2: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f(mut t: s) -> i32 {
// CHECK-NEXT:     let mut c: i32 = t.__slate_bits_0.__get_t();
// CHECK-NEXT:     let mut d: i32 = t.__slate_bits_0.__get_t1();
// CHECK-NEXT:     if c > d {
// CHECK-NEXT:         t.__slate_bits_0.__set_t(d);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         t.__slate_bits_0.__set_t(c);
// CHECK-NEXT:     }
// CHECK-NEXT:     t.__slate_bits_0.__get_t()
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut t: s = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: i32 = -(1 as i32);
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i <= (0 as i32) {
// CHECK-NEXT:         let mut j: i32 = -(1 as i32);
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while j <= (0 as i32) {
// CHECK-NEXT:             let mut t: s = {
// CHECK-NEXT:                 let mut [[T_0:__t[0-9]+]]: s = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_0]].__slate_bits_0.__set_t(i);
// CHECK-NEXT:                 [[T_0]].__slate_bits_0.__set_t1(j);
// CHECK-NEXT:                 [[T_0]]
// CHECK-NEXT:             };
// CHECK-NEXT:             let mut r: i32 = f(t);
// CHECK-NEXT:             let mut exp: i32 = if i < j { i } else { j };
// CHECK-NEXT:             if exp != r {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:             j = [[V_1]];
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
