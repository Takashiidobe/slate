int foo(int i) {
  int a[32];
  a[1] = 3;
  a[0] = 1;
  a[i] = 2;
  return a[0];
}
extern void abort(void);
int         main() {
  if (foo(0) != 2 || foo(1) != 1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn foo(mut i: i32) -> i32 {
// CHECK-NEXT:     let mut a: __SlateAlign16<[i32; 32]> = __SlateAlign16([0 as i32; 32]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (a.0.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) } = 3
// CHECK-NEXT:             as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (a.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } = 1
// CHECK-NEXT:             as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (a.0.as_mut_ptr() as *mut i32).offset(i as isize) } = 2 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { *unsafe { (a.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if foo(0 as i32) != (2 as i32) {
// CHECK-NEXT:         [[V_0]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = foo(1 as i32) != (1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_0]] {
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
