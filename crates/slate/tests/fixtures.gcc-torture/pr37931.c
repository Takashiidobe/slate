/* PR middle-end/37931 */

extern void abort(void);

int foo(int a, unsigned int b) { return (a | 1) & (b | 1); }

int main(void) {
  if (foo(6, 0xc6) != 7)
    abort();
  if (foo(0x80, 0xc1) != 0x81)
    abort();
  if (foo(4, 4) != 5)
    abort();
  if (foo(5, 4) != 5)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/37931
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut a: i32, mut b: u32) -> i32 {
// CHECK-NEXT:     (((a | (1 as i32)) as u32) & (b | ((1 as i32) as u32))) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(6 as i32, (198 as i32) as u32) != (7 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(128 as i32, (193 as i32) as u32) != (129 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(4 as i32, (4 as i32) as u32) != (5 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(5 as i32, (4 as i32) as u32) != (5 as i32) {
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
