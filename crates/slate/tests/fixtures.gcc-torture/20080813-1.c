/* PR middle-end/37103 */

extern void abort(void);

void foo(unsigned short x) {
  signed char y = -1;
  if (x == y)
    abort();
}

void bar(unsigned short x) {
  unsigned char y = -1;
  if (x == y)
    abort();
}

int main(void) {
  if (sizeof(int) == sizeof(short))
    return 0;
  foo(-1);
  if (sizeof(short) > 1)
    bar(-1);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/37103
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: u16) {
// CHECK-NEXT:     let mut y: i8 = -(1 as i32) as i8;
// CHECK-NEXT:     if ((x as u32) as i32) == (y as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: u16) {
// CHECK-NEXT:     let mut y: u8 = (-(1 as i32) as i8) as u8;
// CHECK-NEXT:     if ((x as u32) as i32) == ((y as u32) as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (4 as u64) == (2 as u64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     foo((-(1 as i32) as i16) as u16);
// CHECK-NEXT:     if (2 as u64) > (((1 as i32) as i64) as u64) {
// CHECK-NEXT:         bar((-(1 as i32) as i16) as u16);
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
