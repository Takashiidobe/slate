typedef struct __attribute__((__may_alias__)) {
  short x;
} test;

int f() {
  int   a = 10;
  test *p = (test *)&a;
  p->x    = 1;
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
// CHECK-NEXT: fn f() -> i32 {
// CHECK-NEXT:     let mut a: i32 = 10 as i32;
// CHECK-NEXT:     let mut p: *mut [[SLATERECORD_0]] = std::ptr::addr_of_mut!(a) as *mut [[SLATERECORD_0]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*p).x = (1 as i32) as i16;
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
