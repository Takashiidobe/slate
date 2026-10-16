void        exit(int);
int        *x;
static void bar(char a[2][(*x)++]) {}
int         main(void) { exit(0); }

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: static mut x: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn bar(mut a: &[i8; 0]) {
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: *mut i32 = unsafe { x };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = unsafe { *[[V_1]] };
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_1]] = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
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
