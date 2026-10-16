struct a {
  char b[100];
};
int foo(struct a *a) {
  if (&a->b)
    return 1;
  return 0;
}
extern void abort(void);
int         main() {
  if (foo((struct a *)0) != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct a {
// CHECK-NEXT:     b: [i8; 100],
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut a: *mut a) -> i32 {
// CHECK-NEXT:     if (unsafe { std::ptr::addr_of_mut!((* a).b) }) != std::ptr::null_mut::<[i8; 100]>()
// CHECK-NEXT:     {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(std::ptr::null_mut::<a>()) != (0 as i32) {
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
