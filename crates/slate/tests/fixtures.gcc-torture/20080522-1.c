/* This testcase is to make sure we have i in referenced vars and that we
   properly compute aliasing for the loads and stores.  */

extern void abort(void);

static int  i;
static int *p = &i;

int __attribute__((noinline)) foo(int *q) {
  *p = 1;
  *q = 2;
  return *p;
}

int __attribute__((noinline)) bar(int *q) {
  *q = 2;
  *p = 1;
  return *q;
}

int main() {
  int j = 0;

  if (foo(&i) != 2)
    abort();
  if (bar(&i) != 1)
    abort();
  if (foo(&j) != 1)
    abort();
  if (j != 2)
    abort();
  if (bar(&j) != 2)
    abort();
  if (j != 2)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! This testcase is to make sure we have i in referenced vars and that we
// CHECK-NEXT: //! properly compute aliasing for the loads and stores.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut p: *mut i32 = unsafe { std::ptr::addr_of_mut!(i) };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut q: *mut i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { p } = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *q = 2 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { *unsafe { p } }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut q: *mut i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *q = 2 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { p } = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { *q }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     if foo(unsafe { std::ptr::addr_of_mut!(i) }) != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if bar(unsafe { std::ptr::addr_of_mut!(i) }) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(std::ptr::addr_of_mut!(j)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if j != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if bar(std::ptr::addr_of_mut!(j)) != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if j != (2 as i32) {
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
