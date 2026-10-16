/* PR optimization/8423.  */

void abort(void);
void exit(int);

#define btest(x) __builtin_constant_p(x) ? "1" : "0"

#ifdef __OPTIMIZE__
void foo(char *i) {
  if (*i == '0')
    abort();
}
#else
void foo(char *i) {}
#endif

int main(void) {
  int size = sizeof(int);
  foo(btest(size));
  foo(btest(size));
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR optimization/8423.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut i: *mut i8) {}
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo(if (0 as i32) != (0 as i32) {
// CHECK-NEXT:         c"1".as_ptr() as *mut i8
// CHECK-NEXT:     } else {
// CHECK-NEXT:         c"0".as_ptr() as *mut i8
// CHECK-NEXT:     });
// CHECK-NEXT:     foo(if (0 as i32) != (0 as i32) {
// CHECK-NEXT:         c"1".as_ptr() as *mut i8
// CHECK-NEXT:     } else {
// CHECK-NEXT:         c"0".as_ptr() as *mut i8
// CHECK-NEXT:     });
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
