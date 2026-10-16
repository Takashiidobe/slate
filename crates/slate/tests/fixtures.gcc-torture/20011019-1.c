extern void exit(int);
extern void abort(void);

struct {
  int a;
  int b[5];
} x;
int *y;

int foo(void) { return y - x.b; }

int main(void) {
  y = x.b;
  if (foo())
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: [i32; 5],
// CHECK-NEXT: }
// CHECK-NEXT: static mut x: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut y: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn foo() -> i32 {
// CHECK-NEXT:     ((unsafe {
// CHECK-NEXT:         unsafe { y }
// CHECK-NEXT:             .offset_from(
// CHECK-NEXT:                 (unsafe { std::ptr::addr_of_mut!(x.b) as *mut i32 }) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:     }) as i64) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         y = unsafe { std::ptr::addr_of_mut!(x.b) as *mut i32 };
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo() != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
