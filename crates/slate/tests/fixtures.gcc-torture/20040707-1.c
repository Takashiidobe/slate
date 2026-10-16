void exit(int);

struct s {
  char c1, c2;
};
void foo(struct s s) {
  static struct s s1;
  s1 = s;
}
int main() {
  static struct s s2;
  foo(s2);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct s {
// CHECK-NEXT:     c1: i8,
// CHECK-NEXT:     c2: i8,
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut s: s) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         s1 = s;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut s1: s = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo(unsafe { s2 });
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: static mut s2: s = unsafe { std::mem::zeroed() };
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
