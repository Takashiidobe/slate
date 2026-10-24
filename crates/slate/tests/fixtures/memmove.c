#include <stdio.h>
#include <string.h>

static int get_count(void) { return 4; }

// @check-fn-begin
int main(void) {
  unsigned char forward_buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  memmove(forward_buf + 1, forward_buf, 5);

  unsigned char backward_buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  memmove(backward_buf, backward_buf + 1, 5);

  unsigned char dyn_buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  int           n          = get_count();
  memmove(dyn_buf, dyn_buf + 1, n);

  for (int i = 0; i < 8; i++)
    printf("%d ", forward_buf[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", backward_buf[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", dyn_buf[i]);
  printf("\n");
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut forward_buf: [u8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy(
// CHECK-NEXT:             forward_buf.as_mut_ptr() as *const u8,
// CHECK-NEXT:             ((forward_buf.as_mut_ptr() as *mut u8).add(1) as *mut ()) as *mut u8,
// CHECK-NEXT:             5 as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut backward_buf: [u8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy(
// CHECK-NEXT:             ((backward_buf.as_mut_ptr() as *mut u8).add(1) as *const ()) as *const u8,
// CHECK-NEXT:             backward_buf.as_mut_ptr() as *mut u8,
// CHECK-NEXT:             5 as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut dyn_buf: [u8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// CHECK-NEXT:     let n: i32 = get_count();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy(
// CHECK-NEXT:             ((dyn_buf.as_mut_ptr() as *mut u8).add(1) as *const ()) as *const u8,
// CHECK-NEXT:             dyn_buf.as_mut_ptr() as *mut u8,
// CHECK-NEXT:             ((n as i64) as u64) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     for i in 0i32..8i32 {
// CHECK-NEXT:         print!("{} ", (forward_buf[i as usize] as u32) as i32);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0i32..8i32 {
// CHECK-NEXT:         print!("{} ", (backward_buf[i as usize] as u32) as i32);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0i32..8i32 {
// CHECK-NEXT:         print!("{} ", (dyn_buf[i as usize] as u32) as i32);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
