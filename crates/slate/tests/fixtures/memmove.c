#include <stdio.h>
#include <string.h>

static int get_count(void) { return 4; }

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
// REWRITES-DAG: fn memmove(_0: *mut core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> *mut core::ffi::c_void;
// REWRITES-NOT: safe fn memmove(
// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: forward_buf.copy_within(0..5, 1);
// REWRITES-DAG: backward_buf.copy_within(1..6, 0);
// REWRITES-DAG: unsafe { memmove(
// REWRITES-DAG: dyn_buf.as_mut_ptr()
// REWRITES: {{^}}}
