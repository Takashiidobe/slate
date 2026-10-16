/* { dg-do run }

   # AVR doubles are floats
   { dg-skip-if "AVR doubles are floats" { avr-*-* } } */
extern void abort();

typedef union {
  struct {
    unsigned int hi;
    unsigned int lo;
  } i;
  double d;
} hexdouble;

static const double twoTo52 = 0x1.0p+52;

void func(double x) {
  hexdouble       argument;
  register double y, z;
  unsigned int    xHead;
  argument.d = x;
  xHead      = argument.i.hi & 0x7fffffff;
  if (__builtin_expect(!!(xHead < 0x43300000u), 1)) {
    y = (x - twoTo52) + twoTo52;
    if (y != x)
      abort();
    z = x - 0.5;
    y = (z - twoTo52) + twoTo52;
    if (y == ((x - twoTo52) + twoTo52))
      abort();
  }
  return;
}

int main() {
  if (sizeof(double) == 4)
    return 0;
  func((double)1.00);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn __builtin_expect([[V_0:__v[0-9]+]]: i64, [[V_1:__v[0-9]+]]: i64) -> i64;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-do run }
// CHECK-NEXT: //
// CHECK-NEXT: // # AVR doubles are floats
// CHECK-NEXT: // { dg-skip-if "AVR doubles are floats" { avr-*-* } }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     i: [[SLATERECORD_1:__SlateRecord[0-9]+]],
// CHECK-NEXT:     d: f64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_1]] {
// CHECK-NEXT:     hi: u32,
// CHECK-NEXT:     lo: u32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut twoTo52: f64 = 4503599627370496.0f64;
// CHECK-EMPTY:
// CHECK-NEXT: fn func(mut x: f64) {
// CHECK-NEXT:     let mut argument: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut y: f64 = 0 as f64;
// CHECK-NEXT:     let mut z: f64 = 0 as f64;
// CHECK-NEXT:     let mut xHead: u32 = 0 as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         argument.d = x;
// CHECK-NEXT:     }
// CHECK-NEXT:     xHead = (unsafe { argument.i.hi }) & ((2147483647 as i32) as u32);
// CHECK-NEXT:     if (!(!(xHead < (1127219200 as u32))) as i64) != (0 as i64) {
// CHECK-NEXT:         y = x - unsafe { twoTo52 } + unsafe { twoTo52 };
// CHECK-NEXT:         if y != x {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         z = x - 0.5f64;
// CHECK-NEXT:         y = z - unsafe { twoTo52 } + unsafe { twoTo52 };
// CHECK-NEXT:         if y == x - unsafe { twoTo52 } + unsafe { twoTo52 } {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     return;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (8 as u64) == (((4 as i32) as i64) as u64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     func(1.0f64);
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
