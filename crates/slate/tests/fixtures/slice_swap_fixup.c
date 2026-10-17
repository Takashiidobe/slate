#include <stdio.h>

static void pairwise_swap(int *items, int len) {
  for (int i = 0; i < len; i++) {
    for (int j = 0; j < len; j++) {
      if (items[i] > items[j]) {
        int tmp  = items[i];
        items[i] = items[j];
        items[j] = tmp;
      }
    }
  }
}

static void pairwise_swap_tmp_reused(int *items, int len) {
  for (int i = 0; i < len; i++) {
    for (int j = 0; j < len; j++) {
      if (items[i] > items[j]) {
        int tmp  = items[i];
        items[i] = items[j];
        items[j] = tmp;
        printf("tmp=%d\n", tmp);
      }
    }
  }
}

static void nested_self_swap(int *items, int len) {
  for (int i = 0; i < len; i++) {
    for (int j = 0; j < len; j++) {
      if (items[i] > 0) {
        int tmp  = items[j];
        items[j] = items[j];
        items[j] = tmp;
      }
    }
  }
}

int main(void) {
  int a[5] = {5, 3, 4, 1, 2};
  pairwise_swap(a, 5);
  printf("%d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4]);

  int b[3] = {3, 1, 2};
  pairwise_swap_tmp_reused(b, 3);
  printf("%d %d %d\n", b[0], b[1], b[2]);

  int c[3] = {7, 8, 9};
  nested_self_swap(c, 3);
  printf("%d %d %d\n", c[0], c[1], c[2]);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn pairwise_swap(items: *mut i32, len: i32) {
// CHECK-NEXT:     for i in 0..len {
// CHECK-NEXT:         for j in 0..len {
// CHECK-NEXT:             if (unsafe { *unsafe { items.offset(i as isize) } })
// CHECK-NEXT:                 > unsafe { *unsafe { items.offset(j as isize) } }
// CHECK-NEXT:             {
// CHECK-NEXT:                 let tmp: i32 = unsafe { *unsafe { items.offset(i as isize) } };
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { items.offset(i as isize) } = unsafe {
// CHECK-NEXT:                         *unsafe { items.offset(j as isize) }
// CHECK-NEXT:                     };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { items.offset(j as isize) } = tmp;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn pairwise_swap_tmp_reused(items: *mut i32, len: i32) {
// CHECK-NEXT:     for i in 0..len {
// CHECK-NEXT:         for j in 0..len {
// CHECK-NEXT:             if (unsafe { *unsafe { items.offset(i as isize) } })
// CHECK-NEXT:                 > unsafe { *unsafe { items.offset(j as isize) } }
// CHECK-NEXT:             {
// CHECK-NEXT:                 let tmp: i32 = unsafe { *unsafe { items.offset(i as isize) } };
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { items.offset(i as isize) } = unsafe {
// CHECK-NEXT:                         *unsafe { items.offset(j as isize) }
// CHECK-NEXT:                     };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { items.offset(j as isize) } = tmp;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 println!("tmp={}", tmp);
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn nested_self_swap(items: *mut i32, len: i32) {
// CHECK-NEXT:     for i in 0..len {
// CHECK-NEXT:         for j in 0..len {
// CHECK-NEXT:             if (unsafe { *unsafe { items.offset(i as isize) } }) > 0 {
// CHECK-NEXT:                 let tmp: i32 = unsafe { *unsafe { items.offset(j as isize) } };
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { items.offset(j as isize) } = unsafe {
// CHECK-NEXT:                         *unsafe { items.offset(j as isize) }
// CHECK-NEXT:                     };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { items.offset(j as isize) } = tmp;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: __SlateAlign16<[i32; 5]> = __SlateAlign16([
// CHECK-NEXT:         5 as i32,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     pairwise_swap(a.0.as_mut_ptr() as *mut i32, 5 as i32);
// CHECK-NEXT:     println!("{} {} {} {} {}", a.0[0], a.0[1], a.0[2], a.0[3], a.0[4]);
// CHECK-NEXT:     let mut b: [i32; 3] = [3, 1, 2];
// CHECK-NEXT:     pairwise_swap_tmp_reused(b.as_mut_ptr() as *mut i32, 3 as i32);
// CHECK-NEXT:     println!("{} {} {}", b[0], b[1], b[2]);
// CHECK-NEXT:     let mut c: [i32; 3] = [7, 8, 9];
// CHECK-NEXT:     nested_self_swap(c.as_mut_ptr() as *mut i32, 3 as i32);
// CHECK-NEXT:     println!("{} {} {}", c[0], c[1], c[2]);
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
