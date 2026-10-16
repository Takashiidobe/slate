extern void abort(void);
extern int  strcmp(const char *, const char *);

int foo(const char *a) { return strcmp(a, "main"); }

int main(void) {
  if (foo(__FUNCTION__))
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn strcmp([[V_0:__v[0-9]+]]: *const i8, [[V_1:__v[0-9]+]]: *const i8) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut a: *const i8) -> i32 {
// CHECK-NEXT:     unsafe { strcmp(a, c"main".as_ptr() as *const i8) }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(c"main".as_ptr() as *const i8) != (0 as i32) {
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
