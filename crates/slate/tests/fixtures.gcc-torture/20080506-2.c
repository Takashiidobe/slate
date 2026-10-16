/* PR middle-end/36013 */

extern void abort(void);

void __attribute__((noinline)) foo(int **__restrict p, int **__restrict q) {
  *p[0] = 1;
  *q[0] = 2;
  if (*p[0] != 2)
    abort();
}

int main(void) {
  int  a;
  int *p1 = &a, *p2 = &a;
  foo(&p1, &p2);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/36013
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut p: *mut *mut i32, mut q: *mut *mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { *unsafe { p.offset((0 as i32) as isize) } } = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { *unsafe { q.offset((0 as i32) as isize) } } = 2 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { *unsafe { *unsafe { p.offset((0 as i32) as isize) } } }) != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: i32 = 0 as i32;
// CHECK-NEXT:     let mut p1: *mut i32 = std::ptr::addr_of_mut!(a);
// CHECK-NEXT:     let mut p2: *mut i32 = std::ptr::addr_of_mut!(a);
// CHECK-NEXT:     foo(std::ptr::addr_of_mut!(p1), std::ptr::addr_of_mut!(p2));
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
