/* On H8/300 port, NOTICE_UPDATE_CC had a bug that causes the final
   pass to remove test insns that should be kept.  */

void abort(void);
void exit(int);

unsigned short test1(unsigned short w) {
  if ((w & 0xff00) == 0) {
    if (w == 0)
      w = 2;
  }
  return w;
}

unsigned long test2(unsigned long w) {
  if ((w & 0xffff0000) == 0) {
    if (w == 0)
      w = 2;
  }
  return w;
}

int test3(unsigned short a) {
  if (a & 1)
    return 1;
  else if (a)
    return 1;
  else
    return 0;
}

int main() {
  if (test1(1) != 1)
    abort();

  if (test2(1) != 1)
    abort();

  if (test3(2) != 1)
    abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! On H8/300 port, NOTICE_UPDATE_CC had a bug that causes the final
// CHECK-NEXT: //! pass to remove test insns that should be kept.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test1(mut w: u16) -> u16 {
// CHECK-NEXT:     if ((w as u32) as i32) & (65280 as i32) == (0 as i32) {
// CHECK-NEXT:         if ((w as u32) as i32) == (0 as i32) {
// CHECK-NEXT:             w = ((2 as i32) as i16) as u16;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     w
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test2(mut w: u64) -> u64 {
// CHECK-NEXT:     if w & ((4294901760 as u32) as u64) == (((0 as i32) as i64) as u64) {
// CHECK-NEXT:         if w == (((0 as i32) as i64) as u64) {
// CHECK-NEXT:             w = ((2 as i32) as i64) as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     w
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test3(mut a: u16) -> i32 {
// CHECK-NEXT:     if ((a as u32) as i32) & (1 as i32) != (0 as i32) {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if a != (0 as u16) {
// CHECK-NEXT:             return 1 as i32;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             return 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if ((test1(((1 as i32) as i16) as u16) as u32) as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test2(((1 as i32) as i64) as u64) != (((1 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test3(((2 as i32) as i16) as u16) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
// SLATE-FILECHECK-END
