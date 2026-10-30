#include <stdio.h>

struct S {
  unsigned a;
  unsigned b : 5;
  signed c : 11;
  unsigned d : 16;
};

union U {
  unsigned word;
  unsigned low : 12;
};

volatile struct S gs = {1, 3, -7, 0xbeef};
volatile union U  gu = {0x12345678};

static int calls;

static int next(int v) {
  calls++;
  return v;
}

int main(void) {
  volatile struct S ls = {2, 9, 100, 7};
  volatile union U  lu = {0xabcdef};
  printf("%u %d %u\n", gs.b, gs.c, gs.d);
  printf("%u %u\n", gu.low, lu.low);
  gs.c = next(-300);
  gs.b += 4;
  ls.d = next(0x1ffff);
  lu.low = 0x55;
  gu.low |= 0x800;
  printf("%u %d %u %u\n", gs.a, gs.c, gs.b, gs.d);
  printf("%u %d %u %u\n", ls.b, ls.c, ls.d, ls.a);
  printf("%x %x %d\n", lu.word, gu.word, calls);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     a: u32,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union U {
// CHECK-NEXT:     word: u32,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits1U0,
// CHECK-NEXT: }
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
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
// CHECK-NEXT:         #[bits(5)]
// CHECK-NEXT:         pub b: u32,
// CHECK-NEXT:         #[bits(11)]
// CHECK-NEXT:         pub c: i32,
// CHECK-NEXT:         #[bits(16)]
// CHECK-NEXT:         pub d: u32,
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
// CHECK-NEXT:     pub struct __SlateBits1U0 {
// CHECK-NEXT:         #[bits(12)]
// CHECK-NEXT:         pub low: u32,
// CHECK-NEXT:         #[bits(4, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut gs: S = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].a = 1u32;
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(3u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(-7i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_d(48879u32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut gu: U = {
// CHECK-NEXT:     let mut [[T_0]]: U = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].word = 305419896u32;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut calls: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn next(v: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls = calls + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     v
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut ls: S = {
// CHECK-NEXT:         let mut [[T_0]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].a = 2u32;
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_b(9u32);
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_c(100i32);
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_d(7u32);
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut lu: U = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: U = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].word = 11259375u32;
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %d %u\n".as_ptr(),
// CHECK-NEXT:             std::ptr::read_volatile(
// CHECK-NEXT:                     std::ptr::addr_of!(* std::ptr::addr_of!(gs.__slate_bits_0)),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .__get_b() as i32,
// CHECK-NEXT:             std::ptr::read_volatile(
// CHECK-NEXT:                     std::ptr::addr_of!(* std::ptr::addr_of!(gs.__slate_bits_0)),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .__get_c(),
// CHECK-NEXT:             std::ptr::read_volatile(
// CHECK-NEXT:                     std::ptr::addr_of!(* std::ptr::addr_of!(gs.__slate_bits_0)),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .__get_d() as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %u\n".as_ptr(),
// CHECK-NEXT:             std::ptr::read_volatile(
// CHECK-NEXT:                     std::ptr::addr_of!(* std::ptr::addr_of!(gu.__slate_bits_0)),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .__get_low() as i32,
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(lu.__slate_bits_0)).__get_low()
// CHECK-NEXT:                 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let [[T_2:__t[0-9]+]] = next(-300i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let mut [[T_3:__t[0-9]+]] = std::ptr::read_volatile(
// CHECK-NEXT:             std::ptr::addr_of_mut!(* std::ptr::addr_of_mut!(gs.__slate_bits_0)),
// CHECK-NEXT:         );
// CHECK-NEXT:         [[T_3]].__set_c([[T_2]]);
// CHECK-NEXT:         std::ptr::write_volatile(
// CHECK-NEXT:             std::ptr::addr_of_mut!(* std::ptr::addr_of_mut!(gs.__slate_bits_0)),
// CHECK-NEXT:             [[T_3]],
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: u32 = unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(
// CHECK-NEXT:                 std::ptr::addr_of!(* std::ptr::addr_of!(gs.__slate_bits_0)),
// CHECK-NEXT:             )
// CHECK-NEXT:             .__get_b()
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[T_4:__t[0-9]+]] = (([[V_0]] as i32) + 4) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let mut [[T_5:__t[0-9]+]] = std::ptr::read_volatile(
// CHECK-NEXT:             std::ptr::addr_of_mut!(* std::ptr::addr_of_mut!(gs.__slate_bits_0)),
// CHECK-NEXT:         );
// CHECK-NEXT:         [[T_5]].__set_b([[T_4]]);
// CHECK-NEXT:         std::ptr::write_volatile(
// CHECK-NEXT:             std::ptr::addr_of_mut!(* std::ptr::addr_of_mut!(gs.__slate_bits_0)),
// CHECK-NEXT:             [[T_5]],
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[T_6:__t[0-9]+]] = next(131071i32) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let mut [[T_7:__t[0-9]+]] = std::ptr::read_volatile(
// CHECK-NEXT:             std::ptr::addr_of_mut!(ls.__slate_bits_0),
// CHECK-NEXT:         );
// CHECK-NEXT:         [[T_7]].__set_d([[T_6]]);
// CHECK-NEXT:         std::ptr::write_volatile(std::ptr::addr_of_mut!(ls.__slate_bits_0), [[T_7]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[T_8:__t[0-9]+]] = 85u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let mut [[T_9:__t[0-9]+]] = std::ptr::read_volatile(
// CHECK-NEXT:             std::ptr::addr_of_mut!(lu.__slate_bits_0),
// CHECK-NEXT:         );
// CHECK-NEXT:         [[T_9]].__set_low([[T_8]]);
// CHECK-NEXT:         std::ptr::write_volatile(std::ptr::addr_of_mut!(lu.__slate_bits_0), [[T_9]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: u32 = unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(
// CHECK-NEXT:                 std::ptr::addr_of!(* std::ptr::addr_of!(gu.__slate_bits_0)),
// CHECK-NEXT:             )
// CHECK-NEXT:             .__get_low()
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[T_10:__t[0-9]+]] = (([[V_1]] as i32) | 2048) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let mut [[T_11:__t[0-9]+]] = std::ptr::read_volatile(
// CHECK-NEXT:             std::ptr::addr_of_mut!(* std::ptr::addr_of_mut!(gu.__slate_bits_0)),
// CHECK-NEXT:         );
// CHECK-NEXT:         [[T_11]].__set_low([[T_10]]);
// CHECK-NEXT:         std::ptr::write_volatile(
// CHECK-NEXT:             std::ptr::addr_of_mut!(* std::ptr::addr_of_mut!(gu.__slate_bits_0)),
// CHECK-NEXT:             [[T_11]],
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %d %u %u\n".as_ptr(),
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(gs.a)),
// CHECK-NEXT:             std::ptr::read_volatile(
// CHECK-NEXT:                     std::ptr::addr_of!(* std::ptr::addr_of!(gs.__slate_bits_0)),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .__get_c(),
// CHECK-NEXT:             std::ptr::read_volatile(
// CHECK-NEXT:                     std::ptr::addr_of!(* std::ptr::addr_of!(gs.__slate_bits_0)),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .__get_b() as i32,
// CHECK-NEXT:             std::ptr::read_volatile(
// CHECK-NEXT:                     std::ptr::addr_of!(* std::ptr::addr_of!(gs.__slate_bits_0)),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .__get_d() as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %d %u %u\n".as_ptr(),
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(ls.__slate_bits_0)).__get_b()
// CHECK-NEXT:                 as i32,
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(ls.__slate_bits_0)).__get_c(),
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(ls.__slate_bits_0)).__get_d()
// CHECK-NEXT:                 as i32,
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(ls.a)),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%x %x %d\n".as_ptr(),
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(lu.word)),
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(gu.word)),
// CHECK-NEXT:             calls,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
