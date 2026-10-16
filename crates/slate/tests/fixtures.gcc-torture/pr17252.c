/* PR 17252.  When a char * pointer P takes its own address, storing
   into *P changes P itself.  */

void abort(void);

char *a;

int main(void) {
  /* Make 'a' point to itself.  */
  a = (char *)&a;

  /* Change what 'a' is pointing to.  */
  a[0]++;

  /* If a's memory tag does not contain 'a' in its alias set, we will
     think that this predicate is superfluous and change it to
     'if (1)'.  */
  if (a == (char *)&a)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR 17252.  When a char * pointer P takes its own address, storing
// CHECK-NEXT: //! into *P changes P itself.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     // Make 'a' point to itself.
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = (unsafe { std::ptr::addr_of_mut!(a) }) as *mut i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     // Change what 'a' is pointing to.
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: *mut i8 = unsafe { unsafe { a }.offset((0 as i32) as isize) };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i8 = unsafe { *[[V_0]] };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i8 = (([[V_1]] as i32) + (1 as i32)) as i8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_0]] = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     // If a's memory tag does not contain 'a' in its alias set, we will
// CHECK-NEXT:     // think that this predicate is superfluous and change it to
// CHECK-NEXT:     // 'if (1)'.
// CHECK-NEXT:     if (unsafe { a }) == ((unsafe { std::ptr::addr_of_mut!(a) }) as *mut i8) {
// CHECK-NEXT:         unsafe { abort() };
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
