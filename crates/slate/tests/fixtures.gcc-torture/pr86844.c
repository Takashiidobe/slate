/* PR tree-optimization/86844 */

__attribute__((noipa)) void foo(int *p) {
  *p               = 0;
  *((char *)p + 3) = 1;
  *((char *)p + 1) = 2;
  *((char *)p + 2) = *((char *)p + 6);
}

int main() {
  int a[2] = {-1, 0};
  if (sizeof(int) != 4)
    return 0;
  ((char *)a)[6] = 3;
  foo(a);
  if (((char *)a)[0] != 0 || ((char *)a)[1] != 2 || ((char *)a)[2] != 3 ||
      ((char *)a)[3] != 1)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/86844
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut p: *mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *p = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (p as *mut i8).offset((3 as i32) as isize) } = (1 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (p as *mut i8).offset((1 as i32) as isize) } = (2 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (p as *mut i8).offset((2 as i32) as isize) } =
// CHECK-NEXT:             unsafe { *unsafe { (p as *mut i8).offset((6 as i32) as isize) } };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: [i32; 2] = [-(1 as i32), 0 as i32];
// CHECK-NEXT:     if (4 as u64) != (((4 as i32) as i64) as u64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (a.as_mut_ptr() as *mut i8).offset((6 as i32) as isize) } = (3 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     foo(a.as_mut_ptr() as *mut i32);
// CHECK-NEXT:     if ((unsafe { *unsafe { (a.as_mut_ptr() as *mut i8).offset((0 as i32) as isize) } }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (a.as_mut_ptr() as *mut i8).offset((1 as i32) as isize) } }) as i32)
// CHECK-NEXT:             != (2 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (a.as_mut_ptr() as *mut i8).offset((2 as i32) as isize) } }) as i32)
// CHECK-NEXT:             != (3 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { (a.as_mut_ptr() as *mut i8).offset((3 as i32) as isize) } }) as i32)
// CHECK-NEXT:             != (1 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
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
