int __attribute__((noinline)) bar(int *a) { return *a; }
int                           i;
int __attribute__((noinline)) foo(int (*a)[2]) { return bar(&(*a)[i]); }

extern void abort(void);
int         a[2];
int         main() {
  a[0] = -1;
  a[1] = 42;
  i    = 1;
  if (foo(&a) != 42)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn bar(mut [[A_0:a_[0-9]+]]: *mut i32) -> i32 {
// CHECK-NEXT:     unsafe { *[[A_0]] }
// CHECK-NEXT: }
// CHECK-NEXT: static mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn foo(mut [[A_1:a_[0-9]+]]: *mut [i32; 2]) -> i32 {
// CHECK-NEXT:     bar(unsafe {
// CHECK-NEXT:         unsafe { (*[[A_1]]).as_mut_ptr() as *mut i32 }.offset((unsafe { i }) as isize)
// CHECK-NEXT:     })
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: [i32; 2] = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((0 as i32) as isize)
// CHECK-NEXT:         } = -(1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((1 as i32) as isize)
// CHECK-NEXT:         } = 42 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         i = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(unsafe { std::ptr::addr_of_mut!(a) }) != (42 as i32) {
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
