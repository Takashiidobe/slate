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

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut equal_a: [u8; 4] = [
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:         ((2 as i32) as i8) as u8,
// CHECK-NEXT:         ((3 as i32) as i8) as u8,
// CHECK-NEXT:         ((4 as i32) as i8) as u8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     let mut equal_b: [u8; 4] = [
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:         ((2 as i32) as i8) as u8,
// CHECK-NEXT:         ((3 as i32) as i8) as u8,
// CHECK-NEXT:         ((4 as i32) as i8) as u8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     let mut equal_result: i32 = ((unsafe {
// CHECK-NEXT:         memcmp(
// CHECK-NEXT:             equal_a.as_mut_ptr() as *const (),
// CHECK-NEXT:             equal_b.as_mut_ptr() as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) == (0 as i32)) as i32;
// CHECK-NEXT:     let mut unequal_a: [u8; 4] = [
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:         ((2 as i32) as i8) as u8,
// CHECK-NEXT:         ((3 as i32) as i8) as u8,
// CHECK-NEXT:         ((4 as i32) as i8) as u8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     let mut unequal_b: [u8; 4] = [
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:         ((2 as i32) as i8) as u8,
// CHECK-NEXT:         ((3 as i32) as i8) as u8,
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     let mut unequal_result: i32 = ((unsafe {
// CHECK-NEXT:         memcmp(
// CHECK-NEXT:             unequal_a.as_mut_ptr() as *const (),
// CHECK-NEXT:             unequal_b.as_mut_ptr() as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) == (0 as i32)) as i32;
// CHECK-NEXT:     let mut partial_a: [u8; 8] = [
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:         ((2 as i32) as i8) as u8,
// CHECK-NEXT:         ((3 as i32) as i8) as u8,
// CHECK-NEXT:         ((4 as i32) as i8) as u8,
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     let mut partial_b: [u8; 8] = [
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:         ((2 as i32) as i8) as u8,
// CHECK-NEXT:         ((3 as i32) as i8) as u8,
// CHECK-NEXT:         ((4 as i32) as i8) as u8,
// CHECK-NEXT:         ((0 as i32) as i8) as u8,
// CHECK-NEXT:         ((0 as i32) as i8) as u8,
// CHECK-NEXT:         ((0 as i32) as i8) as u8,
// CHECK-NEXT:         ((0 as i32) as i8) as u8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     let mut partial_result: i32 = ((unsafe {
// CHECK-NEXT:         memcmp(
// CHECK-NEXT:             partial_a.as_mut_ptr() as *const (),
// CHECK-NEXT:             partial_b.as_mut_ptr() as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) == (0 as i32)) as i32;
// CHECK-NEXT:     let mut dyn_a: [u8; 8] = [
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:         ((2 as i32) as i8) as u8,
// CHECK-NEXT:         ((3 as i32) as i8) as u8,
// CHECK-NEXT:         ((4 as i32) as i8) as u8,
// CHECK-NEXT:         ((5 as i32) as i8) as u8,
// CHECK-NEXT:         ((6 as i32) as i8) as u8,
// CHECK-NEXT:         ((7 as i32) as i8) as u8,
// CHECK-NEXT:         ((8 as i32) as i8) as u8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     let mut dyn_b: [u8; 8] = [
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:         ((2 as i32) as i8) as u8,
// CHECK-NEXT:         ((3 as i32) as i8) as u8,
// CHECK-NEXT:         ((4 as i32) as i8) as u8,
// CHECK-NEXT:         ((5 as i32) as i8) as u8,
// CHECK-NEXT:         ((6 as i32) as i8) as u8,
// CHECK-NEXT:         ((7 as i32) as i8) as u8,
// CHECK-NEXT:         ((8 as i32) as i8) as u8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     let mut n: i32 = get_count();
// CHECK-NEXT:     let mut dyn_result: i32 = ((unsafe {
// CHECK-NEXT:         memcmp(
// CHECK-NEXT:             dyn_a.as_mut_ptr() as *const (),
// CHECK-NEXT:             dyn_b.as_mut_ptr() as *const (),
// CHECK-NEXT:             (n as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) == (0 as i32)) as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d %d\n".as_ptr() as *const i8,
// CHECK-NEXT:             equal_result,
// CHECK-NEXT:             unequal_result,
// CHECK-NEXT:             partial_result,
// CHECK-NEXT:             dyn_result,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
