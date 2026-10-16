/* Undefined behavior from a call to a function cast to a different
   type does not appear until after the function designator and
   arguments have been evaluated.  PR 38483.  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */
/* { dg-require-effective-target untyped_assembly } */

extern void exit(int);
extern void abort(void);

int foo(void) {
  exit(0);
  return 0;
}

void bar(void) {}

int main(void) {
  ((long (*)(int))bar)(foo());
  abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Undefined behavior from a call to a function cast to a different
// CHECK-NEXT: //! type does not appear until after the function designator and
// CHECK-NEXT: //! arguments have been evaluated.  PR 38483.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Origin: Joseph Myers <joseph@codesourcery.com>
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-require-effective-target untyped_assembly }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() -> i32 {
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn bar() {}
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn()>,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(i32) -> i64>,
// CHECK-NEXT:             >(Some(bar))
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()(foo())
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { abort() };
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
