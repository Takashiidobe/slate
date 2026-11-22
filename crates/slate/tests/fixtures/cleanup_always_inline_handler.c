#include <stdio.h>

static int released;

static inline __attribute__((always_inline)) void unlock(int *depth) {
  __asm__ volatile("" ::: "memory");
  released += *depth;
  *depth = 0;
}

static int guarded(int depth, int bail) {
  int held __attribute__((cleanup(unlock))) = depth;
  if (bail)
    return -held;
  held += 10;
  return held;
}

static int skipped(int n) {
  if (n > 2)
    goto out;
  {
    int held __attribute__((cleanup(unlock))) = n;
    released += 100;
  }
out:
  return released;
}

int main(void) {
  printf("%d\n", guarded(3, 0));
  printf("%d\n", guarded(4, 1));
  printf("%d\n", skipped(5));
  printf("%d\n", skipped(1));
  printf("%d\n", released);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: static mut released: i32 = unsafe { core::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: fn unlock(depth: *mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!("", options(nostack, att_syntax));
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         released = released + *depth;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *depth = 0;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn guarded(depth: i32, bail: i32) -> i32 {
// CHECK-NEXT:     let mut held: i32 = depth;
// CHECK-NEXT:     let __slate_cleanup_ptr_0 = core::ptr::addr_of_mut!(held) as *mut _;
// CHECK-NEXT:     let __slate_cleanup_0 = ::slate_support::Guard(
// CHECK-NEXT:         &mut || unlock(__slate_cleanup_ptr_0 as *mut _),
// CHECK-NEXT:     );
// CHECK-NEXT:     if bail != 0 {
// CHECK-NEXT:         return -held;
// CHECK-NEXT:     }
// CHECK-NEXT:     held += 10;
// CHECK-NEXT:     held
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn skipped(n: i32) -> i32 {
// CHECK-NEXT:     if !(n > 2) {
// CHECK-NEXT:         let mut held: i32 = n;
// CHECK-NEXT:         let __slate_cleanup_ptr_1 = core::ptr::addr_of_mut!(held) as *mut _;
// CHECK-NEXT:         let __slate_cleanup_1 = ::slate_support::Guard(
// CHECK-NEXT:             &mut || unlock(__slate_cleanup_ptr_1 as *mut _),
// CHECK-NEXT:         );
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             released = released + 100;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { released }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", guarded(3i32, 0i32));
// CHECK-NEXT:     println!("{}", guarded(4i32, 1i32));
// CHECK-NEXT:     println!("{}", skipped(5i32));
// CHECK-NEXT:     println!("{}", skipped(1i32));
// CHECK-NEXT:     println!("{}", unsafe { released });
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
