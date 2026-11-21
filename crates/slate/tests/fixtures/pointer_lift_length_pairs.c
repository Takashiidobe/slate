#include <stdio.h>
#include <stdlib.h>

static const int table[4] = {3, 1, 4, 1};

static void fill(int *values, int len) {
  for (int i = 0; i < len; ++i)
    values[i] = i * 3;
}

static void forward(int *values, int len) { fill(values, len); }

static void bump(int *values, int len) {
  for (int i = 0; i < len; ++i)
    values[i] += 1;
}

static int total(const int *values, int len) {
  int sum = 0;
  for (int i = 0; i < len; ++i) {
    __builtin_prefetch(values + i + 2);
    sum += values[i];
  }
  return sum;
}

static int prefix(const int *values, int len) {
  int sum = 0;
  for (int i = 0; i < len; ++i)
    sum += values[i];
  return sum;
}

int main(void) {
  int  len  = 5;
  int *heap = malloc(len * sizeof(int));
  forward(heap, len);
  int local[3] = {7, 8, 9};
  bump(local, 3);
  int window[4] = {1, 2, 3, 4};
  printf("%d %d %d %d\n", heap[4], total(local, 3), total(table, 4),
         prefix(window, 2));
  free(heap);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __builtin_prefetch([[ARG_0:arg[0-9]+]]: *const (), ...);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlign16<T> {}
// CHECK-EMPTY:
// CHECK-NEXT: static table: __SlateAlign16<[i32; 4]> = __SlateAlign16([3i32, 1i32, 4i32, 1i32]);
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::malloc;
// CHECK-NEXT: use slate_support::free;
// CHECK-EMPTY:
// CHECK-NEXT: fn fill(values: &mut [i32]) {
// CHECK-NEXT:     let len: i32 = values.len() as i32;
// CHECK-NEXT:     for i in 0..len {
// CHECK-NEXT:         values[i as usize] = i * 3;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn forward(values: &mut [i32]) {
// CHECK-NEXT:     fill(values);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bump(values: &mut [i32]) {
// CHECK-NEXT:     let len: i32 = values.len() as i32;
// CHECK-NEXT:     for i in 0..len {
// CHECK-NEXT:         values[i as usize] += 1;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn total(values: &[i32]) -> i32 {
// CHECK-NEXT:     let len: i32 = values.len() as i32;
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     for i in 0..len {
// CHECK-NEXT:         sum += values[i as usize];
// CHECK-NEXT:     }
// CHECK-NEXT:     sum
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn prefix(values: &[i32], len: i32) -> i32 {
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     for i in 0..len {
// CHECK-NEXT:         sum += values[i as usize];
// CHECK-NEXT:     }
// CHECK-NEXT:     sum
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let len: i32 = 5;
// CHECK-NEXT:     let heap: *mut i32 = (unsafe {
// CHECK-NEXT:         malloc((len as u64).wrapping_mul(core::mem::size_of::<i32>() as u64) as usize)
// CHECK-NEXT:             as *mut ()
// CHECK-NEXT:     }) as *mut i32;
// CHECK-NEXT:     forward(unsafe { core::slice::from_raw_parts_mut(heap, len as usize) });
// CHECK-NEXT:     let mut local: [i32; 3] = [7, 8, 9];
// CHECK-NEXT:     bump(local.as_mut_slice());
// CHECK-NEXT:     let mut window: __SlateAlign16<[i32; 4]> = __SlateAlign16([1i32, 2i32, 3i32, 4i32]);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}",
// CHECK-NEXT:         unsafe { *heap.add(4) },
// CHECK-NEXT:         total(local.as_slice()),
// CHECK-NEXT:         total(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::slice::from_raw_parts(
// CHECK-NEXT:                     core::ptr::addr_of!(table.0) as *const i32,
// CHECK-NEXT:                     4 as usize,
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         ),
// CHECK-NEXT:         prefix(window.0.as_slice(), 2i32)
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe { free((heap as *mut ()) as *mut core::ffi::c_void) };
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
