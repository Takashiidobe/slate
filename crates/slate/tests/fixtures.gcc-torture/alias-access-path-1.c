/* Test that variable
     int val;
   may hold value of tyope "struct c" which has same size.
   This is valid in GIMPLE memory model.  */

struct a {
  int val;
} a = {1}, a2;
struct b {
  struct a a;
};
int val;
struct c {
  struct b b;
} *cptr = (void *)&val;

int main(void) {
  cptr->b.a = a;
  val       = 2;
  a2        = cptr->b.a;
  if (a2.val == a.val)
    __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Test that variable
// CHECK-NEXT: //!   int val;
// CHECK-NEXT: //! may hold value of tyope "struct c" which has same size.
// CHECK-NEXT: //! This is valid in GIMPLE memory model.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct a {
// CHECK-NEXT:     val: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: a = a { val: 1 as i32 };
// CHECK-EMPTY:
// CHECK-NEXT: static mut a2: a = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct b {
// CHECK-NEXT:     a: a,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut val: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct c {
// CHECK-NEXT:     b: b,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut cptr: *mut c = ((unsafe { std::ptr::addr_of_mut!(val) }) as *mut ()) as *mut c;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { cptr }).b.a = unsafe { a };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         val = 2 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a2 = unsafe { (*unsafe { cptr }).b.a };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { a2.val }) == unsafe { a.val } {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
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
