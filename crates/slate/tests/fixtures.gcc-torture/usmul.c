/* { dg-require-effective-target int32plus } */
void abort(void);
void exit(int);

int __attribute__((noinline)) foo(short x, unsigned short y) { return x * y; }

int __attribute__((noinline)) bar(unsigned short x, short y) { return x * y; }

int main() {
  if (foo(-2, 0xffff) != -131070)
    abort();
  if (foo(2, 0xffff) != 131070)
    abort();
  if (foo(-32768, 0x8000) != -1073741824)
    abort();
  if (foo(32767, 0x8000) != 1073709056)
    abort();

  if (bar(0xffff, -2) != -131070)
    abort();
  if (bar(0xffff, 2) != 131070)
    abort();
  if (bar(0x8000, -32768) != -1073741824)
    abort();
  if (bar(0x8000, 32767) != 1073709056)
    abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-require-effective-target int32plus }
// CHECK-NEXT: fn foo(mut x: i16, mut y: u16) -> i32 {
// CHECK-NEXT:     (x as i32) * ((y as u32) as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: u16, mut y: i16) -> i32 {
// CHECK-NEXT:     ((x as u32) as i32) * (y as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(-(2 as i32) as i16, ((65535 as i32) as i16) as u16) != -(131070 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo((2 as i32) as i16, ((65535 as i32) as i16) as u16) != (131070 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(-(32768 as i32) as i16, ((32768 as i32) as i16) as u16) != -(1073741824 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo((32767 as i32) as i16, ((32768 as i32) as i16) as u16) != (1073709056 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if bar(((65535 as i32) as i16) as u16, -(2 as i32) as i16) != -(131070 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if bar(((65535 as i32) as i16) as u16, (2 as i32) as i16) != (131070 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if bar(((32768 as i32) as i16) as u16, -(32768 as i32) as i16) != -(1073741824 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if bar(((32768 as i32) as i16) as u16, (32767 as i32) as i16) != (1073709056 as i32) {
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
