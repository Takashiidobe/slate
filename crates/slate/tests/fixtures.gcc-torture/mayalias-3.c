typedef struct __attribute__((__may_alias__)) {
  short x;
} test;

test *p;

int g(int *a) { p = (test *)a; }

int f() {
  int a;
  g(&a);
  a      = 10;
  test s = {1};
  *p     = s;
  return a;
}

int main() {
  if (f() == 10)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     x: i16,
// CHECK-NEXT: }
// CHECK-NEXT: static mut p: *mut [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn g(mut a: *mut i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         p = a as *mut [[SLATERECORD_0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn f() -> i32 {
// CHECK-NEXT:     let mut a: i32 = 0 as i32;
// CHECK-NEXT:     g(std::ptr::addr_of_mut!(a));
// CHECK-NEXT:     a = 10 as i32;
// CHECK-NEXT:     let mut s: [[SLATERECORD_0]] = [[SLATERECORD_0]] {
// CHECK-NEXT:         x: (1 as i32) as i16,
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { p } = s;
// CHECK-NEXT:     }
// CHECK-NEXT:     a
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if f() == (10 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
