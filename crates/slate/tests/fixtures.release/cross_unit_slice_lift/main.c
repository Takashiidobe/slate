// SLATE-ENV: SLATE_IR_POINTERS=1
#include <stdio.h>
#include <stdlib.h>

int  table_sum(const int *values, int count);
void table_scale(int *values, int count, int by);
int  table_first(const int *values);

static int window_total(void) {
  extern int table_sum(const int *values, int count);
  int window[2] = {5, 6};
  return table_sum(window, 2);
}

int main(void) {
  int values[4] = {1, 2, 3, 4};
  table_scale(values, 4, 3);
  int  count = 3;
  int *heap  = malloc(count * sizeof(int));
  for (int i = 0; i < count; ++i)
    heap[i] = i + 10;
  table_scale(heap, count, 2);
  printf("%d %d %d %d\n", table_sum(values, 4), window_total(), table_sum(heap, count),
         table_first(values));
  free(heap);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! SLATE-ENV: SLATE_IR_POINTERS=1
// CHECK-NEXT: #[path = "table.rs"]
// CHECK-NEXT: mod __slate_unit_table;
// CHECK-EMPTY:
// CHECK-NEXT: use crate::__slate_unit_table::table_first;
// CHECK-NEXT: use crate::__slate_unit_table::table_scale;
// CHECK-NEXT: use crate::__slate_unit_table::table_sum;
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlign16<T> {}
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::malloc;
// CHECK-NEXT: use slate_support::free;
// CHECK-EMPTY:
// CHECK-NEXT: fn window_total() -> i32 {
// CHECK-NEXT:     let mut window: [i32; 2] = [5, 6];
// CHECK-NEXT:     unsafe { table_sum(window.as_slice()) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut values: __SlateAlign16<[i32; 4]> = __SlateAlign16([1i32, 2i32, 3i32, 4i32]);
// CHECK-NEXT:     unsafe { table_scale(values.0.as_mut_slice(), 3i32) };
// CHECK-NEXT:     let count: i32 = 3;
// CHECK-NEXT:     let heap: *mut i32 = (unsafe {
// CHECK-NEXT:         malloc((count as u64).wrapping_mul(core::mem::size_of::<i32>() as u64) as usize)
// CHECK-NEXT:             as *mut ()
// CHECK-NEXT:     }) as *mut i32;
// CHECK-NEXT:     for i in 0..count {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *heap.offset(i as isize) = i + 10;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { table_scale(core::slice::from_raw_parts_mut(heap, count as usize), 2i32) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}",
// CHECK-NEXT:         unsafe { table_sum(values.0.as_slice()) },
// CHECK-NEXT:         window_total(),
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             table_sum(core::slice::from_raw_parts(heap as *const i32, count as usize))
// CHECK-NEXT:         },
// CHECK-NEXT:         unsafe { table_first(values.0.as_slice()) }
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
