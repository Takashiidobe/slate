/* PR 3177 */
/* Produced a SIGILL on ia64 with sibcall from F to G.  We hadn't
   widened the register window to allow for the fourth outgoing
   argument as an "in" register.  */

float g(void *a, void *b, int e, int c, float d) { return d; }

float f(void *a, void *b, int c, float d) { return g(a, b, 0, c, d); }

int main() {
  f(0, 0, 1, 1);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR 3177
// CHECK-NEXT: //!
// CHECK-NEXT: //! Produced a SIGILL on ia64 with sibcall from F to G.  We hadn't
// CHECK-NEXT: //! widened the register window to allow for the fourth outgoing
// CHECK-NEXT: //! argument as an "in" register.
// CHECK-NEXT: fn g(mut a: *mut (), mut b: *mut (), mut e: i32, mut c: i32, mut d: f32) -> f32 {
// CHECK-NEXT:     d
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut a: *mut (), mut b: *mut (), mut c: i32, mut d: f32) -> f32 {
// CHECK-NEXT:     g(a, b, 0 as i32, c, d)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     f(
// CHECK-NEXT:         std::ptr::null_mut::<()>(),
// CHECK-NEXT:         std::ptr::null_mut::<()>(),
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         (1 as i32) as f32,
// CHECK-NEXT:     );
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
