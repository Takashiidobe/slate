#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int get_n(void) { return 3; }

// @check-fn-begin
int main(void) {
  unsigned char dst_a[8] = {0};
  unsigned char src_a[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  memcpy(dst_a, src_a, 4);

  unsigned char dst_b[8] = {9, 9, 9, 9, 9, 9, 9, 9};
  unsigned char src_b[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  memmove(dst_b, src_b, 5);

  unsigned char self_c[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  memmove(self_c, self_c, 8);

  unsigned char dst_d[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  memset(dst_d, 0x41, 5);

  unsigned char dst_e[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  unsigned char src_e[8] = {9, 9, 9, 9, 9, 9, 9, 9};
  memcpy(dst_e, src_e + 2, 4);

  unsigned char dst_f[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  unsigned char src_f[8] = {9, 9, 9, 9, 9, 9, 9, 9};
  int           n        = get_n();
  memcpy(dst_f, src_f, n);

  unsigned char  dst_g[4] = {0, 0, 0, 0};
  unsigned char *src_g    = malloc(4);
  memset(src_g, 5, 4);
  memcpy(dst_g, src_g, 4);
  free(src_g);

  for (int i = 0; i < 8; i++)
    printf("%d ", dst_a[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", dst_b[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", self_c[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", dst_d[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", dst_e[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", dst_f[i]);
  for (int i = 0; i < 4; i++)
    printf("%d ", dst_g[i]);
  printf("\n");
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut dst_a: [u8; 8] = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [u8; 8] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = ((0 as i32) as i8) as u8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut src_a: [u8; 8] = [
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
// CHECK-NEXT:             dst_a.as_mut_ptr() as *mut (),
// CHECK-NEXT:             src_a.as_mut_ptr() as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut dst_b: [u8; 8] = [
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     let mut src_b: [u8; 8] = [
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
// CHECK-NEXT:         memmove(
// CHECK-NEXT:             dst_b.as_mut_ptr() as *mut (),
// CHECK-NEXT:             src_b.as_mut_ptr() as *const (),
// CHECK-NEXT:             ((5 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut self_c: [u8; 8] = [
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
// CHECK-NEXT:         memmove(
// CHECK-NEXT:             self_c.as_mut_ptr() as *mut (),
// CHECK-NEXT:             self_c.as_mut_ptr() as *const (),
// CHECK-NEXT:             ((8 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut dst_d: [u8; 8] = [
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
// CHECK-NEXT:         memset(dst_d.as_mut_ptr() as *mut (), 65 as i32, ((5 as i32) as i64) as u64)
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut dst_e: [u8; 8] = [
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:         ((2 as i32) as i8) as u8,
// CHECK-NEXT:         ((3 as i32) as i8) as u8,
// CHECK-NEXT:         ((4 as i32) as i8) as u8,
// CHECK-NEXT:         ((5 as i32) as i8) as u8,
// CHECK-NEXT:         ((6 as i32) as i8) as u8,
// CHECK-NEXT:         ((7 as i32) as i8) as u8,
// CHECK-NEXT:         ((8 as i32) as i8) as u8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     let mut src_e: [u8; 8] = [
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             dst_e.as_mut_ptr() as *mut (),
// CHECK-NEXT:             (unsafe { (src_e.as_mut_ptr() as *mut u8).offset((2 as i32) as isize) })
// CHECK-NEXT:                 as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut dst_f: [u8; 8] = [
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:         ((2 as i32) as i8) as u8,
// CHECK-NEXT:         ((3 as i32) as i8) as u8,
// CHECK-NEXT:         ((4 as i32) as i8) as u8,
// CHECK-NEXT:         ((5 as i32) as i8) as u8,
// CHECK-NEXT:         ((6 as i32) as i8) as u8,
// CHECK-NEXT:         ((7 as i32) as i8) as u8,
// CHECK-NEXT:         ((8 as i32) as i8) as u8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     let mut src_f: [u8; 8] = [
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     let mut n: i32 = get_n();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             dst_f.as_mut_ptr() as *mut (),
// CHECK-NEXT:             src_f.as_mut_ptr() as *const (),
// CHECK-NEXT:             (n as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut dst_g: [u8; 4] = [
// CHECK-NEXT:         ((0 as i32) as i8) as u8,
// CHECK-NEXT:         ((0 as i32) as i8) as u8,
// CHECK-NEXT:         ((0 as i32) as i8) as u8,
// CHECK-NEXT:         ((0 as i32) as i8) as u8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     let mut src_g: *mut u8 = (unsafe { malloc(((4 as i32) as i64) as u64) }) as *mut u8;
// CHECK-NEXT:     unsafe { memset(src_g as *mut (), 5 as i32, ((4 as i32) as i64) as u64) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             dst_g.as_mut_ptr() as *mut (),
// CHECK-NEXT:             src_g as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { free(src_g as *mut ()) };
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (8 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%d ".as_ptr() as *const i8,
// CHECK-NEXT:                     ((unsafe {
// CHECK-NEXT:                         *unsafe { (dst_a.as_mut_ptr() as *mut u8).offset(i as isize) }
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
// CHECK-NEXT:                         *unsafe { (dst_b.as_mut_ptr() as *mut u8).offset(i as isize) }
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
// CHECK-NEXT:                         *unsafe { (self_c.as_mut_ptr() as *mut u8).offset(i as isize) }
// CHECK-NEXT:                     }) as u32) as i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:             let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:             i = [[V_5]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while i < (8 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%d ".as_ptr() as *const i8,
// CHECK-NEXT:                     ((unsafe {
// CHECK-NEXT:                         *unsafe { (dst_d.as_mut_ptr() as *mut u8).offset(i as isize) }
// CHECK-NEXT:                     }) as u32) as i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             let mut [[V_6:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:             let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]] + (1 as i32);
// CHECK-NEXT:             i = [[V_7]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: while i < (8 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%d ".as_ptr() as *const i8,
// CHECK-NEXT:                     ((unsafe {
// CHECK-NEXT:                         *unsafe { (dst_e.as_mut_ptr() as *mut u8).offset(i as isize) }
// CHECK-NEXT:                     }) as u32) as i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             let mut [[V_8:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:             let mut [[V_9:__v[0-9]+]]: i32 = [[V_8]] + (1 as i32);
// CHECK-NEXT:             i = [[V_9]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: while i < (8 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%d ".as_ptr() as *const i8,
// CHECK-NEXT:                     ((unsafe {
// CHECK-NEXT:                         *unsafe { (dst_f.as_mut_ptr() as *mut u8).offset(i as isize) }
// CHECK-NEXT:                     }) as u32) as i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             let mut [[V_10:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:             let mut [[V_11:__v[0-9]+]]: i32 = [[V_10]] + (1 as i32);
// CHECK-NEXT:             i = [[V_11]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_6:__slate_break_[0-9]+]]: while i < (4 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%d ".as_ptr() as *const i8,
// CHECK-NEXT:                 ((unsafe {
// CHECK-NEXT:                     *unsafe { (dst_g.as_mut_ptr() as *mut u8).offset(i as isize) }
// CHECK-NEXT:                 }) as u32) as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_12:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_13:__v[0-9]+]]: i32 = [[V_12]] + (1 as i32);
// CHECK-NEXT:         i = [[V_13]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { printf(c"\n".as_ptr() as *const i8) };
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
