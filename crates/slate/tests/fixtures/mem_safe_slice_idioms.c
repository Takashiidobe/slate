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
// CHECK-NEXT:     let src_a: [u8; 8] = [
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:         ((2 as i32) as i8) as u8,
// CHECK-NEXT:         ((3 as i32) as i8) as u8,
// CHECK-NEXT:         ((4 as i32) as i8) as u8,
// CHECK-NEXT:         ((5 as i32) as i8) as u8,
// CHECK-NEXT:         ((6 as i32) as i8) as u8,
// CHECK-NEXT:         ((7 as i32) as i8) as u8,
// CHECK-NEXT:         ((8 as i32) as i8) as u8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     unsafe { dst_a[(0usize..4usize)].copy_from_slice(&src_a[(0usize..4usize)]) };
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
// CHECK-NEXT:     let src_b: [u8; 8] = [
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:         ((2 as i32) as i8) as u8,
// CHECK-NEXT:         ((3 as i32) as i8) as u8,
// CHECK-NEXT:         ((4 as i32) as i8) as u8,
// CHECK-NEXT:         ((5 as i32) as i8) as u8,
// CHECK-NEXT:         ((6 as i32) as i8) as u8,
// CHECK-NEXT:         ((7 as i32) as i8) as u8,
// CHECK-NEXT:         ((8 as i32) as i8) as u8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     unsafe { dst_b[(0usize..5usize)].copy_from_slice(&src_b[(0usize..5usize)]) };
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
// CHECK-NEXT:     unsafe { self_c.copy_within(0usize..8usize, 0usize) };
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
// CHECK-NEXT:     unsafe { dst_d[(0usize..5usize)].fill((65 as i32) as u8) };
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
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             ((unsafe { (src_e.as_mut_ptr() as *mut u8).offset((2 as i32) as isize) })
// CHECK-NEXT:                 as *const ()) as *const u8,
// CHECK-NEXT:             dst_e.as_mut_ptr() as *mut u8,
// CHECK-NEXT:             (((4 as i32) as i64) as u64) as usize,
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
// CHECK-NEXT:     let n: i32 = get_n();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             src_f.as_mut_ptr() as *const u8,
// CHECK-NEXT:             dst_f.as_mut_ptr() as *mut u8,
// CHECK-NEXT:             ((n as i64) as u64) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut dst_g: [u8; 4] = [
// CHECK-NEXT:         ((0 as i32) as i8) as u8,
// CHECK-NEXT:         ((0 as i32) as i8) as u8,
// CHECK-NEXT:         ((0 as i32) as i8) as u8,
// CHECK-NEXT:         ((0 as i32) as i8) as u8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     let src_g: *mut u8 = (unsafe { malloc(((4 as i32) as i64) as u64) }) as *mut u8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_bytes(
// CHECK-NEXT:             src_g as *mut u8,
// CHECK-NEXT:             (5 as i32) as u8,
// CHECK-NEXT:             (((4 as i32) as i64) as u64) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             src_g as *const u8,
// CHECK-NEXT:             dst_g.as_mut_ptr() as *mut u8,
// CHECK-NEXT:             (((4 as i32) as i64) as u64) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { free(src_g as *mut ()) };
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: i32 = 0 as i32;
// CHECK-NEXT:         while i < (8 as i32) {
// CHECK-NEXT:             print!(
// CHECK-NEXT:                 "{} ", ((unsafe { * unsafe { (dst_a.as_mut_ptr() as * mut u8).offset(i as
// CHECK-NEXT:                 isize) } }) as u32) as i32
// CHECK-NEXT:             );
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             i += 1 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: i32 = 0 as i32;
// CHECK-NEXT:         while i < (8 as i32) {
// CHECK-NEXT:             print!(
// CHECK-NEXT:                 "{} ", ((unsafe { * unsafe { (dst_b.as_mut_ptr() as * mut u8).offset(i as
// CHECK-NEXT:                 isize) } }) as u32) as i32
// CHECK-NEXT:             );
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             i += 1 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: i32 = 0 as i32;
// CHECK-NEXT:         while i < (8 as i32) {
// CHECK-NEXT:             print!(
// CHECK-NEXT:                 "{} ", ((unsafe { * unsafe { (self_c.as_mut_ptr() as * mut u8).offset(i
// CHECK-NEXT:                 as isize) } }) as u32) as i32
// CHECK-NEXT:             );
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             i += 1 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: i32 = 0 as i32;
// CHECK-NEXT:         while i < (8 as i32) {
// CHECK-NEXT:             print!(
// CHECK-NEXT:                 "{} ", ((unsafe { * unsafe { (dst_d.as_mut_ptr() as * mut u8).offset(i as
// CHECK-NEXT:                 isize) } }) as u32) as i32
// CHECK-NEXT:             );
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             i += 1 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: i32 = 0 as i32;
// CHECK-NEXT:         while i < (8 as i32) {
// CHECK-NEXT:             print!(
// CHECK-NEXT:                 "{} ", ((unsafe { * unsafe { (dst_e.as_mut_ptr() as * mut u8).offset(i as
// CHECK-NEXT:                 isize) } }) as u32) as i32
// CHECK-NEXT:             );
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             i += 1 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: i32 = 0 as i32;
// CHECK-NEXT:         while i < (8 as i32) {
// CHECK-NEXT:             print!(
// CHECK-NEXT:                 "{} ", ((unsafe { * unsafe { (dst_f.as_mut_ptr() as * mut u8).offset(i as
// CHECK-NEXT:                 isize) } }) as u32) as i32
// CHECK-NEXT:             );
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             i += 1 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     while i < (4 as i32) {
// CHECK-NEXT:         print!(
// CHECK-NEXT:             "{} ", ((unsafe { * unsafe { (dst_g.as_mut_ptr() as * mut u8).offset(i as
// CHECK-NEXT:             isize) } }) as u32) as i32
// CHECK-NEXT:         );
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         i += 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
