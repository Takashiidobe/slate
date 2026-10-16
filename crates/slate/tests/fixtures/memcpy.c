#include <stdio.h>
#include <string.h>

static int get_count(void) { return 4; }

// @check-fn-begin
int main(void) {
  unsigned char full_src[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  unsigned char full_dst[8] = {0};
  memcpy(full_dst, full_src, 8);

  unsigned char partial_src[8] = {9, 8, 7, 6, 5, 4, 3, 2};
  unsigned char partial_dst[8] = {0};
  memcpy(partial_dst, partial_src, 4);

  unsigned char alias_buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  memcpy(alias_buf, alias_buf + 4, 4);

  unsigned char dyn_src[8] = {1, 1, 1, 1, 1, 1, 1, 1};
  unsigned char dyn_dst[8] = {0};
  int           n          = get_count();
  memcpy(dyn_dst, dyn_src, n);

  for (int i = 0; i < 8; i++)
    printf("%d ", full_dst[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", partial_dst[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", alias_buf[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", dyn_dst[i]);
  printf("\n");
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut full_src: [u8; 8] = [
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:         ((2 as i32) as i8) as u8,
// CHECK-NEXT:         ((3 as i32) as i8) as u8,
// CHECK-NEXT:         ((4 as i32) as i8) as u8,
// CHECK-NEXT:         ((5 as i32) as i8) as u8,
// CHECK-NEXT:         ((6 as i32) as i8) as u8,
// CHECK-NEXT:         ((7 as i32) as i8) as u8,
// CHECK-NEXT:         ((8 as i32) as i8) as u8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     let mut full_dst: [u8; 8] = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [u8; 8] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = ((0 as i32) as i8) as u8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             full_dst.as_mut_ptr() as *mut (),
// CHECK-NEXT:             full_src.as_mut_ptr() as *const (),
// CHECK-NEXT:             ((8 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut partial_src: [u8; 8] = [
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:         ((8 as i32) as i8) as u8,
// CHECK-NEXT:         ((7 as i32) as i8) as u8,
// CHECK-NEXT:         ((6 as i32) as i8) as u8,
// CHECK-NEXT:         ((5 as i32) as i8) as u8,
// CHECK-NEXT:         ((4 as i32) as i8) as u8,
// CHECK-NEXT:         ((3 as i32) as i8) as u8,
// CHECK-NEXT:         ((2 as i32) as i8) as u8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     let mut partial_dst: [u8; 8] = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: [u8; 8] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]][0] = ((0 as i32) as i8) as u8;
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             partial_dst.as_mut_ptr() as *mut (),
// CHECK-NEXT:             partial_src.as_mut_ptr() as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut alias_buf: [u8; 8] = [
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:         ((2 as i32) as i8) as u8,
// CHECK-NEXT:         ((3 as i32) as i8) as u8,
// CHECK-NEXT:         ((4 as i32) as i8) as u8,
// CHECK-NEXT:         ((5 as i32) as i8) as u8,
// CHECK-NEXT:         ((6 as i32) as i8) as u8,
// CHECK-NEXT:         ((7 as i32) as i8) as u8,
// CHECK-NEXT:         ((8 as i32) as i8) as u8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             alias_buf.as_mut_ptr() as *mut (),
// CHECK-NEXT:             (unsafe { (alias_buf.as_mut_ptr() as *mut u8).offset((4 as i32) as isize) })
// CHECK-NEXT:                 as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut dyn_src: [u8; 8] = [
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     let mut dyn_dst: [u8; 8] = {
// CHECK-NEXT:         let mut [[T_2:__t[0-9]+]]: [u8; 8] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_2]][0] = ((0 as i32) as i8) as u8;
// CHECK-NEXT:         [[T_2]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut n: i32 = get_count();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             dyn_dst.as_mut_ptr() as *mut (),
// CHECK-NEXT:             dyn_src.as_mut_ptr() as *const (),
// CHECK-NEXT:             (n as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (8 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%d ".as_ptr() as *const i8,
// CHECK-NEXT:                     ((unsafe {
// CHECK-NEXT:                         *unsafe { (full_dst.as_mut_ptr() as *mut u8).offset(i as isize) }
// CHECK-NEXT:                     }) as u32) as i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:             i = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (8 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%d ".as_ptr() as *const i8,
// CHECK-NEXT:                     ((unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             (partial_dst.as_mut_ptr() as *mut u8).offset(i as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as u32) as i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:             i = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while i < (8 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%d ".as_ptr() as *const i8,
// CHECK-NEXT:                     ((unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             (alias_buf.as_mut_ptr() as *mut u8).offset(i as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as u32) as i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:             let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:             i = [[V_5]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while i < (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%d ".as_ptr() as *const i8,
// CHECK-NEXT:                 ((unsafe {
// CHECK-NEXT:                     *unsafe { (dyn_dst.as_mut_ptr() as *mut u8).offset(i as isize) }
// CHECK-NEXT:                 }) as u32) as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]] + (1 as i32);
// CHECK-NEXT:         i = [[V_7]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { printf(c"\n".as_ptr() as *const i8) };
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
