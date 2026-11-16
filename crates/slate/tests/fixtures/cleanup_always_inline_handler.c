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
// CHECK-NEXT: mod __slate_cleanup {
// CHECK-NEXT:     pub struct Guard<'a>(pub &'a mut dyn FnMut());
// CHECK-NEXT:     impl Drop for Guard<'_> {
// CHECK-NEXT:         fn drop(&mut self) {
// CHECK-NEXT:             (self.0)()
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut released: i32 = unsafe { core::mem::zeroed() };
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
// CHECK-NEXT: fn guarded(mut held: i32, bail: i32) -> i32 {
// CHECK-NEXT:     let __slate_cleanup_ptr_0 = core::ptr::addr_of_mut!(held) as *mut _;
// CHECK-NEXT:     let __slate_cleanup_0 = __slate_cleanup::Guard(
// CHECK-NEXT:         &mut || unlock(__slate_cleanup_ptr_0 as *mut _),
// CHECK-NEXT:     );
// CHECK-NEXT:     if bail != 0 {
// CHECK-NEXT:         return -held;
// CHECK-NEXT:     }
// CHECK-NEXT:     held += 10;
// CHECK-NEXT:     held
// CHECK-NEXT: }
// CHECK-NEXT: fn skipped(n: i32) -> i32 {
// CHECK-NEXT:     let mut held: core::mem::MaybeUninit<i32> = core::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut i32 = core::ptr::addr_of_mut!(held) as *mut i32;
// CHECK-NEXT:     let __slate_cleanup_ptr_2 = [[SLATE_SLOT_0]];
// CHECK-NEXT:     let __slate_cleanup_armed_2 = core::cell::Cell::new(false);
// CHECK-NEXT:     let __slate_cleanup_2 = __slate_cleanup::Guard(
// CHECK-NEXT:         &mut || {
// CHECK-NEXT:             if __slate_cleanup_armed_2.get() {
// CHECK-NEXT:                 unsafe { unlock(__slate_cleanup_ptr_2 as *mut _) };
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         if n <= 2 {
// CHECK-NEXT:             core::ptr::write([[SLATE_SLOT_0]], n);
// CHECK-NEXT:             __slate_cleanup_armed_2.set(true);
// CHECK-NEXT:             released = released + 100;
// CHECK-NEXT:             __slate_cleanup_armed_2.set(false);
// CHECK-NEXT:             unlock([[SLATE_SLOT_0]] as *mut _);
// CHECK-NEXT:         }
// CHECK-NEXT:         return released;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", guarded(3i32, 0i32));
// CHECK-NEXT:     println!("{}", guarded(4i32, 1i32));
// CHECK-NEXT:     println!("{}", skipped(5i32));
// CHECK-NEXT:     println!("{}", skipped(1i32));
// CHECK-NEXT:     println!("{}", unsafe { released });
// CHECK-NEXT:     0
// CHECK-NEXT: }
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
