#include <stdio.h>

struct Guard {
  int depth;
};

typedef struct Guard Guard;

int slate_support = 7;

static void release(Guard *guard) { slate_support += guard->depth; }

static int scope(int depth) {
  Guard outer __attribute__((cleanup(release))) = {depth};
  {
    Guard inner __attribute__((cleanup(release))) = {depth * 10};
    slate_support *= 2;
  }
  return slate_support;
}

int main(void) {
  int during = scope(3);
  printf("%d %d\n", during, slate_support);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Guard {
// CHECK-NEXT:     depth: i32,
// CHECK-NEXT: }
// CHECK-NEXT: static mut slate_support: i32 = 7;
// CHECK-NEXT: fn release(guard: *mut Guard) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support = slate_support + (*guard).depth;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn scope(depth: i32) -> i32 {
// CHECK-NEXT:     let mut outer: Guard = Guard { depth: depth };
// CHECK-NEXT:     let __slate_cleanup_ptr_0 = core::ptr::addr_of_mut!(outer) as *mut _;
// CHECK-NEXT:     let __slate_cleanup_0 = ::slate_support::Guard(
// CHECK-NEXT:         &mut || release(__slate_cleanup_ptr_0 as *mut _),
// CHECK-NEXT:     );
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut inner: Guard = Guard { depth: depth * 10 };
// CHECK-NEXT:         let __slate_cleanup_ptr_1 = core::ptr::addr_of_mut!(inner) as *mut _;
// CHECK-NEXT:         let __slate_cleanup_1 = ::slate_support::Guard(
// CHECK-NEXT:             &mut || release(__slate_cleanup_ptr_1 as *mut _),
// CHECK-NEXT:         );
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support = slate_support * 2;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { slate_support }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let during: i32 = scope(3i32);
// CHECK-NEXT:     println!("{} {}", during, unsafe { slate_support });
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
