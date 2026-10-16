extern void abort(void);
void        foo(int *p) {
  int x;
  int y;
  x  = *p;
  *p = 0;
  y  = *p;
  if (x != y)
    return;
  abort();
}

int main() {
  int a = 1;
  foo(&a);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut p: &mut i32) {
// CHECK-NEXT:     let mut x: i32 = unsafe { *(p as *mut i32) };
// CHECK-NEXT:     let mut y: i32 = 0 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(p as *mut i32) = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     y = unsafe { *(p as *mut i32) };
// CHECK-NEXT:     if x != y {
// CHECK-NEXT:         return;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { abort() };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: i32 = 1 as i32;
// CHECK-NEXT:     foo(unsafe { &mut (*std::ptr::addr_of_mut!(a)) });
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
