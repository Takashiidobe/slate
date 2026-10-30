/* { dg-options "-funsigned-bitfields" } */
int printf(const char *, ...);

typedef int plain_int;

struct bits {
  int a : 3;
  plain_int b : 3;
  signed int c : 3;
  char d : 3;
};

int main(void) {
  struct bits v;
  v.a = 7;
  v.b = 7;
  v.c = 7;
  v.d = 7;
  printf("%d %d %d %d\n", v.a, v.b, v.c, v.d);
  printf("%d\n", v.a - 8 < 0);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu i686-unknown-linux-gnu aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const u8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "-funsigned-bitfields" }
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct bits {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits1U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u16,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits1U0 {
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub a: u32,
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub b: u32,
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub c: i32,
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub d: u8,
// CHECK-NEXT:         #[bits(4, access = na)]
// CHECK-NEXT:         pub __slate_pad_4: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut v: bits = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     v.__slate_bits_0.__set_a(7u32);
// CHECK-NEXT:     v.__slate_bits_0.__set_b(7u32);
// CHECK-NEXT:     v.__slate_bits_0.__set_c(7i32);
// CHECK-NEXT:     v.__slate_bits_0.__set_d(7u8);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d %d\n".as_ptr(),
// CHECK-NEXT:             v.__slate_bits_0.__get_a() as i32,
// CHECK-NEXT:             v.__slate_bits_0.__get_b() as i32,
// CHECK-NEXT:             v.__slate_bits_0.__get_c(),
// CHECK-NEXT:             (v.__slate_bits_0.__get_d() as u32) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d\n".as_ptr(),
// CHECK-NEXT:             ((v.__slate_bits_0.__get_a() as i32) - 8 < 0) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     0
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
