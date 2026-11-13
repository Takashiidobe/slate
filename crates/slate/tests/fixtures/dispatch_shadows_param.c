#include <stdio.h>

struct fd {
  unsigned name;
};

static int pick(struct fd *fd, unsigned name, int k) {
  int total = (int)name;
  if (k)
    goto done;
  {
    unsigned name = fd->name;
    total += (int)name * 10;
    if (name > 5) {
      total += 1000;
      goto done;
    }
    total += (int)name;
  }
  total += (int)name * 100;
done:
  return total;
}

int main(void) {
  struct fd small = {3};
  struct fd big = {9};
  printf("%d %d %d\n", pick(&small, 2, 0), pick(&big, 2, 0), pick(&small, 2, 1));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct fd {
// CHECK-NEXT:     name: u32,
// CHECK-NEXT: }
// CHECK-NEXT: fn pick(fd: &fd, name: u32, k: i32) -> i32 {
// CHECK-NEXT:     let mut name__1: u32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut total: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         total = name as i32;
// CHECK-NEXT:         if k == 0 {
// CHECK-NEXT:             name__1 = (*(fd as *const fd)).name;
// CHECK-NEXT:             total += (name__1 as i32) * 10;
// CHECK-NEXT:             if name__1 > 5 {
// CHECK-NEXT:                 total += 1000;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 total += name__1 as i32;
// CHECK-NEXT:                 total += (name as i32) * 100;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         return total;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut small: fd = fd { name: 3 };
// CHECK-NEXT:     let mut big: fd = fd { name: 9 };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", pick(unsafe { & (* std::ptr::addr_of_mut!(small)) }, 2u32, 0i32),
// CHECK-NEXT:         pick(unsafe { & (* std::ptr::addr_of_mut!(big)) }, 2u32, 0i32), pick(unsafe { &
// CHECK-NEXT:         (* std::ptr::addr_of_mut!(small)) }, 2u32, 1i32)
// CHECK-NEXT:     );
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
