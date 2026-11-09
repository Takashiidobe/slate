#include <stdio.h>

static int total;

static void release(int *value) { total += *value; }

static int scope(void) {
  {
    int value __attribute__((cleanup(release))) = 53;
    total += 1;
  }
  int after = total;
  {
    int first __attribute__((cleanup(release)))  = 100;
    int second __attribute__((cleanup(release))) = 1000;
    total *= 2;
  }
  return after * 10000 + total;
}

int main(void) {
  printf("%d\n", scope());
  printf("%d\n", total);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: mod __slate_cleanup {
// CHECK-NEXT:     pub struct Guard<F: FnMut()>(pub F);
// CHECK-NEXT:     impl<F: FnMut()> Drop for Guard<F> {
// CHECK-NEXT:         fn drop(&mut self) {
// CHECK-NEXT:             (self.0)()
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut total: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn release(value: *mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         total = total + *value;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn scope() -> i32 {
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut value: i32 = 53;
// CHECK-NEXT:         let __slate_cleanup_ptr_0 = std::ptr::addr_of_mut!(value) as *mut _;
// CHECK-NEXT:         let __slate_cleanup_0 = __slate_cleanup::Guard(|| release(
// CHECK-NEXT:             __slate_cleanup_ptr_0 as *mut _,
// CHECK-NEXT:         ));
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             total = total + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let after: i32 = unsafe { total };
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut first: i32 = 100;
// CHECK-NEXT:         let __slate_cleanup_ptr_3 = std::ptr::addr_of_mut!(first) as *mut _;
// CHECK-NEXT:         let __slate_cleanup_3 = __slate_cleanup::Guard(|| release(
// CHECK-NEXT:             __slate_cleanup_ptr_3 as *mut _,
// CHECK-NEXT:         ));
// CHECK-NEXT:         let mut second: i32 = 1000;
// CHECK-NEXT:         let __slate_cleanup_ptr_4 = std::ptr::addr_of_mut!(second) as *mut _;
// CHECK-NEXT:         let __slate_cleanup_4 = __slate_cleanup::Guard(|| release(
// CHECK-NEXT:             __slate_cleanup_ptr_4 as *mut _,
// CHECK-NEXT:         ));
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             total = total * 2;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     after * 10000 + unsafe { total }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", scope());
// CHECK-NEXT:     println!("{}", unsafe { total });
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
