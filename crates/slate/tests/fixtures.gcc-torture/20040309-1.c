extern void abort();

int foo(unsigned short x) {
  unsigned short y;
  y = x > 32767 ? x - 32768 : 0;
  return y;
}

int main() {
  if (foo(0) != 0)
    abort();
  if (foo(32767) != 0)
    abort();
  if (foo(32768) != 0)
    abort();
  if (foo(32769) != 1)
    abort();
  if (foo(65535) != 32767)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut x: u16) -> i32 {
// CHECK-NEXT:     let mut y: u16 = ((if ((x as u32) as i32) > (32767 as i32) {
// CHECK-NEXT:         ((x as u32) as i32) - (32768 as i32)
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0 as i32
// CHECK-NEXT:     }) as i16) as u16;
// CHECK-NEXT:     (y as u32) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(((0 as i32) as i16) as u16) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(((32767 as i32) as i16) as u16) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(((32768 as i32) as i16) as u16) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(((32769 as i32) as i16) as u16) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(((65535 as i32) as i16) as u16) != (32767 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
