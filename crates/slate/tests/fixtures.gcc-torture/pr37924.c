/* PR c/37924 */

extern void abort(void);

signed char   a;
unsigned char b;

int test1(void) {
  int c = -1;
  return ((unsigned int)(a ^ c)) >> 9;
}

int test2(void) {
  int c = -1;
  return ((unsigned int)(b ^ c)) >> 9;
}

int main(void) {
  a = 0;
  if (test1() != (-1U >> 9))
    abort();
  a = 0x40;
  if (test1() != (-1U >> 9))
    abort();
  a = 0x80;
  if (test1() != (a < 0) ? 0 : (-1U >> 9))
    abort();
  a = 0xff;
  if (test1() != (a < 0) ? 0 : (-1U >> 9))
    abort();
  b = 0;
  if (test2() != (-1U >> 9))
    abort();
  b = 0x40;
  if (test2() != (-1U >> 9))
    abort();
  b = 0x80;
  if (test2() != (-1U >> 9))
    abort();
  b = 0xff;
  if (test2() != (-1U >> 9))
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR c/37924
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: u8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn test1() -> i32 {
// CHECK-NEXT:     let mut c: i32 = -(1 as i32);
// CHECK-NEXT:     (((((unsafe { a }) as i32) ^ c) as u32) >> (9 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test2() -> i32 {
// CHECK-NEXT:     let mut c: i32 = -(1 as i32);
// CHECK-NEXT:     ((((((unsafe { b }) as u32) as i32) ^ c) as u32) >> (9 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = (0 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (test1() as u32) != (1 as u32).wrapping_neg() >> (9 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = (64 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (test1() as u32) != (1 as u32).wrapping_neg() >> (9 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = (128 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (if test1() != ((((unsafe { a }) as i32) < (0 as i32)) as i32) {
// CHECK-NEXT:         (0 as i32) as u32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         (1 as u32).wrapping_neg() >> (9 as i32)
// CHECK-NEXT:     }) != (0 as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = (255 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (if test1() != ((((unsafe { a }) as i32) < (0 as i32)) as i32) {
// CHECK-NEXT:         (0 as i32) as u32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         (1 as u32).wrapping_neg() >> (9 as i32)
// CHECK-NEXT:     }) != (0 as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = ((0 as i32) as i8) as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (test2() as u32) != (1 as u32).wrapping_neg() >> (9 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = ((64 as i32) as i8) as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (test2() as u32) != (1 as u32).wrapping_neg() >> (9 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = ((128 as i32) as i8) as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (test2() as u32) != (1 as u32).wrapping_neg() >> (9 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = ((255 as i32) as i8) as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (test2() as u32) != (1 as u32).wrapping_neg() >> (9 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
