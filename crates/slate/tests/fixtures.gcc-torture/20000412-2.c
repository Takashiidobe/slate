void abort(void);
void exit(int);

int f(int a, int *y) {
  int x = a;

  if (a == 0)
    return *y;

  return f(a - 1, &x);
}

int main(int argc, char **argv) {
  if (f(100, (int *)0) != 1)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn f(mut a: i32, mut y: *mut i32) -> i32 {
// CHECK-NEXT:     let mut x: i32 = a;
// CHECK-NEXT:     if a == (0 as i32) {
// CHECK-NEXT:         return unsafe { *y };
// CHECK-NEXT:     }
// CHECK-NEXT:     f(a - (1 as i32), std::ptr::addr_of_mut!(x))
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main(mut argc: i32, mut argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     if f(100 as i32, std::ptr::null_mut::<i32>()) != (1 as i32) {
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
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
