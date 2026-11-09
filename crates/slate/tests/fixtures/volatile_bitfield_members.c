#include <stdio.h>

struct flags {
  volatile int g : 3;
  const volatile int h : 4;
  volatile unsigned u : 9;
  int x;
};

struct __attribute__((packed)) wide {
  char tag;
  volatile unsigned long long big : 40;
  volatile long long neg : 20;
};

static struct flags global = {.g = -1, .h = 5, .u = 300, .x = 7};

static void bump(volatile struct flags *f) {
  f->g += 1;
  f->u = f->u * 2 + 1;
}

static int read_h(const volatile struct flags *f) { return f->h; }

int main(void) {
  struct flags w = {.g = 3, .h = -2, .u = 17, .x = 9};
  w.g = 2;
  w.g++;
  bump(&w);
  bump(&global);
  struct wide p = {.tag = 'a', .big = 0x12345678ULL, .neg = -1000};
  p.big += 0x100000000ULL;
  p.neg -= 5;
  printf("%d %d %u %d\n", w.g, read_h(&w), w.u, w.x);
  printf("%d %d %u %d\n", global.g, read_h(&global), global.u, global.x);
  printf("%c %llx %lld %zu\n", p.tag, (unsigned long long)p.big, (long long)p.neg, sizeof p);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct flags {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT:     x: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct wide {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits1U0,
// CHECK-NEXT: }
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
// CHECK-NEXT:     pub struct __SlateBits0U0 {
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub g: i32,
// CHECK-NEXT:         #[bits(4)]
// CHECK-NEXT:         pub h: i32,
// CHECK-NEXT:         #[bits(9)]
// CHECK-NEXT:         pub u: u32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         [u8;8],
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
// CHECK-NEXT:         #[bits(40)]
// CHECK-NEXT:         pub big: u64,
// CHECK-NEXT:         #[bits(20)]
// CHECK-NEXT:         pub neg: i64,
// CHECK-NEXT:         #[bits(4, access = na)]
// CHECK-NEXT:         pub __slate_pad_2: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut global: flags = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: flags = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_g(-1i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_h(5i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_u(300u32);
// CHECK-NEXT:     [[T_0]].x = 7i32;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn bump(f: *mut flags) {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!((* f).__slate_bits_0)).__get_g()
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[T_0]] = [[V_0]] + 1;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]] = std::ptr::read_volatile(
// CHECK-NEXT:             std::ptr::addr_of_mut!((* f).__slate_bits_0),
// CHECK-NEXT:         );
// CHECK-NEXT:         [[T_1]].__set_g([[T_0]]);
// CHECK-NEXT:         std::ptr::write_volatile(std::ptr::addr_of_mut!((* f).__slate_bits_0), [[T_1]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[T_2:__t[0-9]+]] = (((unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!((* f).__slate_bits_0)).__get_u()
// CHECK-NEXT:     }) as i32) * 2 + 1) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let mut [[T_3:__t[0-9]+]] = std::ptr::read_volatile(
// CHECK-NEXT:             std::ptr::addr_of_mut!((* f).__slate_bits_0),
// CHECK-NEXT:         );
// CHECK-NEXT:         [[T_3]].__set_u([[T_2]]);
// CHECK-NEXT:         std::ptr::write_volatile(std::ptr::addr_of_mut!((* f).__slate_bits_0), [[T_3]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn read_h(f: *const flags) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!((* f).__slate_bits_0)).__get_h()
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut w: flags = {
// CHECK-NEXT:         let mut [[T_0]]: flags = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_g(3i32);
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_h(-2i32);
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_u(17u32);
// CHECK-NEXT:         [[T_0]].x = 9i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[T_1]] = 2i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let mut [[T_2]] = std::ptr::read_volatile(std::ptr::addr_of_mut!(w.__slate_bits_0));
// CHECK-NEXT:         [[T_2]].__set_g([[T_1]]);
// CHECK-NEXT:         std::ptr::write_volatile(std::ptr::addr_of_mut!(w.__slate_bits_0), [[T_2]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(w.__slate_bits_0)).__get_g()
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[T_3]] = [[V_1]] + 1;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let mut [[T_4:__t[0-9]+]] = std::ptr::read_volatile(std::ptr::addr_of_mut!(w.__slate_bits_0));
// CHECK-NEXT:         [[T_4]].__set_g([[T_3]]);
// CHECK-NEXT:         std::ptr::write_volatile(std::ptr::addr_of_mut!(w.__slate_bits_0), [[T_4]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     bump(std::ptr::addr_of_mut!(w) as *mut flags);
// CHECK-NEXT:     bump((unsafe { std::ptr::addr_of_mut!(global) }) as *mut flags);
// CHECK-NEXT:     let mut p: wide = {
// CHECK-NEXT:         let mut [[T_5:__t[0-9]+]]: wide = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_5]].tag = 97i8;
// CHECK-NEXT:         [[T_5]].__slate_bits_0.__set_big(305419896u64);
// CHECK-NEXT:         [[T_5]].__slate_bits_0.__set_neg(-1000i64);
// CHECK-NEXT:         [[T_5]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: u64 = unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(p.__slate_bits_0)).__get_big()
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[T_6:__t[0-9]+]] = [[V_2]].wrapping_add(4294967296);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let mut [[T_7:__t[0-9]+]] = std::ptr::read_volatile(std::ptr::addr_of_mut!(p.__slate_bits_0));
// CHECK-NEXT:         [[T_7]].__set_big([[T_6]]);
// CHECK-NEXT:         std::ptr::write_volatile(std::ptr::addr_of_mut!(p.__slate_bits_0), [[T_7]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: i64 = unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(p.__slate_bits_0)).__get_neg()
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[T_8:__t[0-9]+]] = (([[V_3]] as i32) - 5) as i64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let mut [[T_9:__t[0-9]+]] = std::ptr::read_volatile(std::ptr::addr_of_mut!(p.__slate_bits_0));
// CHECK-NEXT:         [[T_9]].__set_neg([[T_8]]);
// CHECK-NEXT:         std::ptr::write_volatile(std::ptr::addr_of_mut!(p.__slate_bits_0), [[T_9]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%d %d %u %d\n".as_ptr(),
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(w.__slate_bits_0)).__get_g(),
// CHECK-NEXT:             read_h(std::ptr::addr_of_mut!(w) as *const flags),
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(w.__slate_bits_0)).__get_u()
// CHECK-NEXT:                 as i32,
// CHECK-NEXT:             w.x,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%d %d %u %d\n".as_ptr(),
// CHECK-NEXT:             std::ptr::read_volatile(
// CHECK-NEXT:                     std::ptr::addr_of!(* std::ptr::addr_of!(global.__slate_bits_0)),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .__get_g(),
// CHECK-NEXT:             read_h(std::ptr::addr_of_mut!(global) as *const flags),
// CHECK-NEXT:             std::ptr::read_volatile(
// CHECK-NEXT:                     std::ptr::addr_of!(* std::ptr::addr_of!(global.__slate_bits_0)),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .__get_u() as i32,
// CHECK-NEXT:             global.x,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%c %llx %lld %zu\n".as_ptr(),
// CHECK-NEXT:             p.tag as i32,
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(p.__slate_bits_0)).__get_big(),
// CHECK-NEXT:             (std::ptr::read_volatile(std::ptr::addr_of!(p.__slate_bits_0)).__get_neg()
// CHECK-NEXT:                 as i32) as i64,
// CHECK-NEXT:             std::mem::size_of::<wide>() as u64,
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
