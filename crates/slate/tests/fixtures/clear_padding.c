#include <stdio.h>
#include <string.h>

struct holes {
  char c;
  int i;
  short s;
  double d;
};

struct nested {
  struct holes inner[2];
  char tail;
};

struct bits {
  int a : 3;
  int : 2;
  int b : 5;
  char c;
};

static void dump(const char *name, const void *p, size_t n) {
  const unsigned char *bytes = p;
  printf("%s:", name);
  for (size_t i = 0; i < n; i++)
    printf(" %02x", bytes[i]);
  printf("\n");
}

int main(void) {
  struct holes h;
  memset(&h, 0xAB, sizeof h);
  h.c = 1;
  h.i = 2;
  h.s = 3;
  h.d = 4.0;
  __builtin_clear_padding(&h);
  dump("holes", &h, sizeof h);

  struct nested n;
  memset(&n, 0xCD, sizeof n);
  n.tail = 9;
  __builtin_clear_padding(&n);
  dump("nested", &n, sizeof n);

  struct bits b;
  memset(&b, 0xFF, sizeof b);
  b.a = 1;
  b.b = 2;
  b.c = 3;
  __builtin_clear_padding(&b);
  dump("bits", &b, sizeof b);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn memset(__s: *mut (), __c: i32, __n: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct holes {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     i: i32,
// CHECK-NEXT:     s: i16,
// CHECK-NEXT:     d: f64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct nested {
// CHECK-NEXT:     inner: [holes; 2],
// CHECK-NEXT:     tail: i8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct bits {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits3U0,
// CHECK-NEXT:     c: i8,
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
// CHECK-NEXT:     pub struct __SlateBits3U0 {
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub a: i32,
// CHECK-NEXT:         #[bits(2, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
// CHECK-NEXT:         #[bits(5)]
// CHECK-NEXT:         pub b: i32,
// CHECK-NEXT:         #[bits(6, access = na)]
// CHECK-NEXT:         pub __slate_pad_3: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn dump(name: *const i8, p: *const (), n: u64) {
// CHECK-NEXT:     let bytes: *const u8 = p as *const u8;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%s:".as_ptr(), name) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c" %02x".as_ptr(), (*bytes.offset(i as isize) as u32) as i32) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut h: holes = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_bytes(
// CHECK-NEXT:             (std::ptr::addr_of_mut!(h) as *mut ()) as *mut u8,
// CHECK-NEXT:             171 as u8,
// CHECK-NEXT:             24 as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     h.c = 1;
// CHECK-NEXT:     h.i = 2;
// CHECK-NEXT:     h.s = 3;
// CHECK-NEXT:     h.d = 4.0f64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: *mut u8 = std::ptr::addr_of_mut!(h) as *mut u8;
// CHECK-NEXT:         [[T_0]].add(1usize).write_bytes(0u8, 3usize);
// CHECK-NEXT:         [[T_0]].add(10usize).write_bytes(0u8, 6usize);
// CHECK-NEXT:     };
// CHECK-NEXT:     dump(c"holes".as_ptr(), std::ptr::addr_of_mut!(h) as *const (), 24 as u64);
// CHECK-NEXT:     let mut n: nested = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_bytes(
// CHECK-NEXT:             (std::ptr::addr_of_mut!(n) as *mut ()) as *mut u8,
// CHECK-NEXT:             205 as u8,
// CHECK-NEXT:             56 as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     n.tail = 9;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: *mut u8 = std::ptr::addr_of_mut!(n) as *mut u8;
// CHECK-NEXT:         [[T_1]].add(1usize).write_bytes(0u8, 3usize);
// CHECK-NEXT:         [[T_1]].add(10usize).write_bytes(0u8, 6usize);
// CHECK-NEXT:         [[T_1]].add(25usize).write_bytes(0u8, 3usize);
// CHECK-NEXT:         [[T_1]].add(34usize).write_bytes(0u8, 6usize);
// CHECK-NEXT:         [[T_1]].add(49usize).write_bytes(0u8, 7usize);
// CHECK-NEXT:     };
// CHECK-NEXT:     dump(c"nested".as_ptr(), std::ptr::addr_of_mut!(n) as *const (), 56 as u64);
// CHECK-NEXT:     let mut b: bits = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_bytes(
// CHECK-NEXT:             (std::ptr::addr_of_mut!(b) as *mut ()) as *mut u8,
// CHECK-NEXT:             255 as u8,
// CHECK-NEXT:             4 as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     b.__slate_bits_0.__set_a(1 as i32);
// CHECK-NEXT:     b.__slate_bits_0.__set_b(2 as i32);
// CHECK-NEXT:     b.c = 3;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: *mut u8 = std::ptr::addr_of_mut!(b) as *mut u8;
// CHECK-NEXT:         *[[T_2]].add(0usize) &= 231u8;
// CHECK-NEXT:         *[[T_2]].add(1usize) &= 3u8;
// CHECK-NEXT:         [[T_2]].add(3usize).write_bytes(0u8, 1usize);
// CHECK-NEXT:     };
// CHECK-NEXT:     dump(c"bits".as_ptr(), std::ptr::addr_of_mut!(b) as *const (), 4 as u64);
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
