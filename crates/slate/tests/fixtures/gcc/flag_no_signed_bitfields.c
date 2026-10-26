/* { dg-options "-fno-signed-bitfields" } */
int printf(const char *, ...);

typedef signed int explicit_int;
typedef explicit_int still_explicit;
typedef int plain;
typedef plain still_plain;

struct bits {
  still_explicit a : 3;
  still_plain b : 3;
  __typeof__(signed int) c : 3;
  short d : 3;
  signed short e : 3;
  signed char f : 3;
  const int g : 3;
  unsigned h : 3;
#ifdef __SIZEOF_INT128__
  __int128_t i : 3;
#endif
};

int main(void) {
  struct bits v;
  v.a = 7;
  v.b = 7;
  v.c = 7;
  v.d = 7;
  v.e = 7;
  v.f = 7;
  v.h = 7;
  printf("%d %d %d %d %d %d %d\n", v.a, v.b, v.c, v.d, v.e, v.f, v.h);
  struct bits w = {.g = 7};
  printf("%d %d\n", w.g, w.g - 8 < 0);
#ifdef __SIZEOF_INT128__
  v.i = 7;
  printf("%d\n", (int)v.i);
#else
  printf("7\n");
#endif
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu i686-unknown-linux-gnu aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn printf([[V_0:__v[0-9]+]]: *const i8, ...) -> i32;
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn printf([[V_0:__v[0-9]+]]: *const i8, ...) -> i32;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     fn printf([[V_0:__v[0-9]+]]: *const u8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "-fno-signed-bitfields" }
// X86_64-UNKNOWN-LINUX-GNU-NEXT: #[repr(C, align(16))]
// I686-UNKNOWN-LINUX-GNU-NEXT: #[repr(C, align(4))]
// AARCH64-UNKNOWN-LINUX-GNU-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct bits {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits4U0,
// CHECK-NEXT:     __slate_bits_1: __slate_bits::__SlateBits4U1,
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
// CHECK-NEXT:     pub struct __SlateBits4U0 {
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub a: i32,
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub b: u32,
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub c: u32,
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub d: u16,
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub e: i16,
// CHECK-NEXT:         #[bits(1, access = na)]
// CHECK-NEXT:         pub __slate_pad_5: u8,
// CHECK-NEXT:     }
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
// CHECK-NEXT:     pub struct __SlateBits4U1 {
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub f: i8,
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub g: u32,
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub h: u32,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         #[bits(3)]
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         pub i: u128,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         #[bits(4, access = na)]
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         pub __slate_pad_4: u8,
// I686-UNKNOWN-LINUX-GNU-NEXT:         #[bits(7, access = na)]
// I686-UNKNOWN-LINUX-GNU-NEXT:         pub __slate_pad_3: u8,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         #[bits(3)]
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         pub i: u128,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         #[bits(4, access = na)]
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         pub __slate_pad_4: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut v: bits = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     v.__slate_bits_0.__set_a(7i32);
// CHECK-NEXT:     v.__slate_bits_0.__set_b(7u32);
// CHECK-NEXT:     v.__slate_bits_0.__set_c(7u32);
// CHECK-NEXT:     v.__slate_bits_0.__set_d(7u16);
// CHECK-NEXT:     v.__slate_bits_0.__set_e(7i16);
// CHECK-NEXT:     v.__slate_bits_1.__set_f(7i8);
// CHECK-NEXT:     v.__slate_bits_1.__set_h(7u32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d %d %d %d %d\n".as_ptr(),
// CHECK-NEXT:             v.__slate_bits_0.__get_a(),
// CHECK-NEXT:             v.__slate_bits_0.__get_b() as i32,
// CHECK-NEXT:             v.__slate_bits_0.__get_c() as i32,
// CHECK-NEXT:             (v.__slate_bits_0.__get_d() as u32) as i32,
// CHECK-NEXT:             v.__slate_bits_0.__get_e() as i32,
// CHECK-NEXT:             v.__slate_bits_1.__get_f() as i32,
// CHECK-NEXT:             v.__slate_bits_1.__get_h() as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut w: bits = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: bits = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].__slate_bits_1.__set_g(7u32);
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d\n".as_ptr(),
// CHECK-NEXT:             w.__slate_bits_1.__get_g() as i32,
// CHECK-NEXT:             ((w.__slate_bits_1.__get_g() as i32) - 8 < 0) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     v.__slate_bits_1.__set_i(7 as u128);
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     unsafe { printf(c"%d\n".as_ptr(), (v.__slate_bits_1.__get_i() as u32) as i32) };
// I686-UNKNOWN-LINUX-GNU-NEXT:     unsafe { printf(c"7\n".as_ptr()) };
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     v.__slate_bits_1.__set_i(7 as u128);
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     unsafe { printf(c"%d\n".as_ptr(), (v.__slate_bits_1.__get_i() as u32) as i32) };
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
