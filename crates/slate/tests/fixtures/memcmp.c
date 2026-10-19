#include <stdio.h>
#include <string.h>

static int get_count(void) { return 4; }

// @check-fn-begin
int main(void) {
  unsigned char equal_a[4]   = {1, 2, 3, 4};
  unsigned char equal_b[4]   = {1, 2, 3, 4};
  int           equal_result = memcmp(equal_a, equal_b, 4) == 0;

  unsigned char unequal_a[4]   = {1, 2, 3, 4};
  unsigned char unequal_b[4]   = {1, 2, 3, 9};
  int           unequal_result = memcmp(unequal_a, unequal_b, 4) == 0;

  unsigned char partial_a[8]   = {1, 2, 3, 4, 9, 9, 9, 9};
  unsigned char partial_b[8]   = {1, 2, 3, 4, 0, 0, 0, 0};
  int           partial_result = memcmp(partial_a, partial_b, 4) == 0;

  unsigned char dyn_a[8]   = {1, 2, 3, 4, 5, 6, 7, 8};
  unsigned char dyn_b[8]   = {1, 2, 3, 4, 5, 6, 7, 8};
  int           n          = get_count();
  int           dyn_result = memcmp(dyn_a, dyn_b, n) == 0;

  printf("%d %d %d %d\n", equal_result, unequal_result, partial_result,
         dyn_result);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut equal_a: [u8; 4] = [1, 2, 3, 4];
// CHECK-NEXT:     let mut equal_b: [u8; 4] = [1, 2, 3, 4];
// CHECK-NEXT:     let equal_result: i32 = ((unsafe {
// CHECK-NEXT:         memcmp(
// CHECK-NEXT:             equal_a.as_mut_ptr() as *const (),
// CHECK-NEXT:             equal_b.as_mut_ptr() as *const (),
// CHECK-NEXT:             4u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) == 0) as i32;
// CHECK-NEXT:     let mut unequal_a: [u8; 4] = [1, 2, 3, 4];
// CHECK-NEXT:     let mut unequal_b: [u8; 4] = [1, 2, 3, 9];
// CHECK-NEXT:     let unequal_result: i32 = ((unsafe {
// CHECK-NEXT:         memcmp(
// CHECK-NEXT:             unequal_a.as_mut_ptr() as *const (),
// CHECK-NEXT:             unequal_b.as_mut_ptr() as *const (),
// CHECK-NEXT:             4u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) == 0) as i32;
// CHECK-NEXT:     let mut partial_a: [u8; 8] = [1, 2, 3, 4, 9, 9, 9, 9];
// CHECK-NEXT:     let mut partial_b: [u8; 8] = [1, 2, 3, 4, 0, 0, 0, 0];
// CHECK-NEXT:     let partial_result: i32 = ((unsafe {
// CHECK-NEXT:         memcmp(
// CHECK-NEXT:             partial_a.as_mut_ptr() as *const (),
// CHECK-NEXT:             partial_b.as_mut_ptr() as *const (),
// CHECK-NEXT:             4u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) == 0) as i32;
// CHECK-NEXT:     let mut dyn_a: [u8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// CHECK-NEXT:     let mut dyn_b: [u8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// CHECK-NEXT:     let n: i32 = get_count();
// CHECK-NEXT:     let dyn_result: i32 = ((unsafe {
// CHECK-NEXT:         memcmp(
// CHECK-NEXT:             dyn_a.as_mut_ptr() as *const (),
// CHECK-NEXT:             dyn_b.as_mut_ptr() as *const (),
// CHECK-NEXT:             (n as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) == 0) as i32;
// CHECK-NEXT:     println!("{} {} {} {}", equal_result, unequal_result, partial_result, dyn_result);
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
