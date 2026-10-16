/* Copyright (C) 2003  Free Software Foundation.
   PR target/13256
   STRICT_LOW_PART was handled incorrectly in delay slots.
   Origin: Hans-Peter Nilsson.  */

void abort(void);
void exit(int);

typedef struct {
  unsigned int e0 : 16;
  unsigned int e1 : 16;
} s1;
typedef struct {
  unsigned int e0 : 16;
  unsigned int e1 : 16;
} s2;
typedef struct {
  s1 i12;
  s2 i16;
} io;
static int           test_length = 2;
static io           *i;
static int           m = 1;
static int           d = 1;
static unsigned long test_t0;
static unsigned long test_t1;
void                 test(void) __attribute__((__noinline__));
extern int           f1(void *port) __attribute__((__noinline__));
extern void          f0(void) __attribute__((__noinline__));
int                  f1(void *port) {
  int           fail_count = 0;
  unsigned long tlen;
  s1            x0 = {0};
  s2            x1 = {0};

  i     = port;
  x0.e0 = x1.e0 = 32;
  i->i12        = x0;
  i->i16        = x1;
  do
    f0();
  while (test_t1);
  x0.e0 = x1.e0 = 8;
  i->i12        = x0;
  i->i16        = x1;
  test();
  if (m) {
    unsigned long e = 1000000000 / 460800 * test_length;
    tlen            = test_t1 - test_t0;
    if (((tlen - e) & 0x7FFFFFFF) > 1000)
      f0();
  }
  if (d) {
    unsigned long e = 1000000000 / 460800 * test_length;
    tlen            = test_t1 - test_t0;
    if (((tlen - e) & 0x7FFFFFFF) > 1000)
      f0();
  }
  return fail_count != 0 ? 1 : 0;
}

int main() {
  io io0;
  f1(&io0);
  abort();
}

void test(void) {
  io *iop = i;
  if (iop->i12.e0 != 8 || iop->i16.e0 != 8)
    abort();
  exit(0);
}

void f0(void) {
  static int washere = 0;
  io        *iop     = i;
  if (washere++ || iop->i12.e0 != 32 || iop->i16.e0 != 32)
    abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Copyright (C) 2003  Free Software Foundation.
// CHECK-NEXT: //! PR target/13256
// CHECK-NEXT: //! STRICT_LOW_PART was handled incorrectly in delay slots.
// CHECK-NEXT: //! Origin: Hans-Peter Nilsson.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_1:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits2U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_2:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     i12: [[SLATERECORD_0]],
// CHECK-NEXT:     i16: [[SLATERECORD_1]],
// CHECK-NEXT: }
// CHECK-EMPTY:
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
// CHECK-NEXT:         #[bits(16)]
// CHECK-NEXT:         pub e0: u32,
// CHECK-NEXT:         #[bits(16)]
// CHECK-NEXT:         pub e1: u32,
// CHECK-NEXT:     }
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
// CHECK-NEXT:     pub struct __SlateBits2U0 {
// CHECK-NEXT:         #[bits(16)]
// CHECK-NEXT:         pub e0: u32,
// CHECK-NEXT:         #[bits(16)]
// CHECK-NEXT:         pub e1: u32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut test_length: i32 = 2 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut i: *mut [[SLATERECORD_2]] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut m: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut test_t0: u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut test_t1: u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn f1(mut port: *mut ()) -> i32 {
// CHECK-NEXT:     let mut fail_count: i32 = 0 as i32;
// CHECK-NEXT:     let mut tlen: u64 = 0 as u64;
// CHECK-NEXT:     let mut x0: [[SLATERECORD_0]] = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_e0((0 as i32) as u32);
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut x1: [[SLATERECORD_1]] = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: [[SLATERECORD_1]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].__slate_bits_0.__set_e0((0 as i32) as u32);
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         i = port as *mut [[SLATERECORD_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     x1.__slate_bits_0.__set_e0((32 as i32) as u32);
// CHECK-NEXT:     x0.__slate_bits_0
// CHECK-NEXT:         .__set_e0((((((32 as i32) as u32) as u16) as u32) as i32) as u32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { i }).i12 = x0;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { i }).i16 = x1;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         f0();
// CHECK-NEXT:         if !((unsafe { test_t1 }) != (0 as u64)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     x1.__slate_bits_0.__set_e0((8 as i32) as u32);
// CHECK-NEXT:     x0.__slate_bits_0
// CHECK-NEXT:         .__set_e0((((((8 as i32) as u32) as u16) as u32) as i32) as u32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { i }).i12 = x0;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { i }).i16 = x1;
// CHECK-NEXT:     }
// CHECK-NEXT:     test();
// CHECK-NEXT:     if (unsafe { m }) != (0 as i32) {
// CHECK-NEXT:         let mut e: u64 =
// CHECK-NEXT:             (((1000000000 as i32) / (460800 as i32) * unsafe { test_length }) as i64) as u64;
// CHECK-NEXT:         tlen = unsafe { test_t1 }.wrapping_sub(unsafe { test_t0 });
// CHECK-NEXT:         if tlen.wrapping_sub(e) & (((2147483647 as i32) as i64) as u64)
// CHECK-NEXT:             > (((1000 as i32) as i64) as u64)
// CHECK-NEXT:         {
// CHECK-NEXT:             f0();
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { d }) != (0 as i32) {
// CHECK-NEXT:         let mut e: u64 =
// CHECK-NEXT:             (((1000000000 as i32) / (460800 as i32) * unsafe { test_length }) as i64) as u64;
// CHECK-NEXT:         tlen = unsafe { test_t1 }.wrapping_sub(unsafe { test_t0 });
// CHECK-NEXT:         if tlen.wrapping_sub(e) & (((2147483647 as i32) as i64) as u64)
// CHECK-NEXT:             > (((1000 as i32) as i64) as u64)
// CHECK-NEXT:         {
// CHECK-NEXT:             f0();
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if fail_count != (0 as i32) {
// CHECK-NEXT:         1 as i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0 as i32
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut io0: [[SLATERECORD_2]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     f1(std::ptr::addr_of_mut!(io0) as *mut ());
// CHECK-NEXT:     unsafe { abort() };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test() {
// CHECK-NEXT:     let mut iop: *mut [[SLATERECORD_2]] = unsafe { i };
// CHECK-NEXT:     if ((unsafe { (*iop).i12.__slate_bits_0.__get_e0() }) as i32) != (8 as i32)
// CHECK-NEXT:         || ((unsafe { (*iop).i16.__slate_bits_0.__get_e0() }) as i32) != (8 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f0() {
// CHECK-NEXT:     let mut iop: *mut [[SLATERECORD_2]] = unsafe { i };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = unsafe { washere };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         washere = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_1]] != (0 as i32)
// CHECK-NEXT:         || ((unsafe { (*iop).i12.__slate_bits_0.__get_e0() }) as i32) != (32 as i32)
// CHECK-NEXT:         || ((unsafe { (*iop).i16.__slate_bits_0.__get_e0() }) as i32) != (32 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut washere: i32 = 0 as i32;
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
