/* A function pointer compared with a void pointer should not be canonicalized.
   See PR middle-end/17564.  */
void abort(void);
void exit(int);

void *f(void) __attribute__((__noinline__));
void *f(void) { return f; }
int   main(void) {
  if (f() != f)
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
// CHECK-NEXT: // A function pointer compared with a void pointer should not be canonicalized.
// CHECK-NEXT: // See PR middle-end/17564.
// CHECK-NEXT: extern "C-unwind" fn f() -> *mut () {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::mem::transmute::<Option<unsafe extern "C-unwind" fn() -> *mut ()>, *mut ()>(Some(f))
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if f()
// CHECK-NEXT:         != unsafe {
// CHECK-NEXT:             std::mem::transmute::<Option<unsafe extern "C-unwind" fn() -> *mut ()>, *mut ()>(Some(
// CHECK-NEXT:                 f,
// CHECK-NEXT:             ))
// CHECK-NEXT:         }
// CHECK-NEXT:     {
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
