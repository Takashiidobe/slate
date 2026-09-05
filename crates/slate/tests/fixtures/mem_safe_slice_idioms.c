#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int get_n(void) { return 3; }

int main(void) {
  unsigned char dst_a[8] = {0};
  unsigned char src_a[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  // @lowering-begin
  // @rewrite-begin
  memcpy(dst_a, src_a, 4);
  // @rewrite-end
  // @lowering-end

  unsigned char dst_b[8] = {9, 9, 9, 9, 9, 9, 9, 9};
  unsigned char src_b[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  // @rewrite-begin
  memmove(dst_b, src_b, 5);
  // @rewrite-end

  unsigned char self_c[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  // @rewrite-begin
  memmove(self_c, self_c, 8);
  // @rewrite-end

  unsigned char dst_d[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  // @rewrite-begin
  memset(dst_d, 0x41, 5);
  // @rewrite-end

  unsigned char dst_e[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  unsigned char src_e[8] = {9, 9, 9, 9, 9, 9, 9, 9};
  // @rewrite-begin
  memcpy(dst_e, src_e + 2, 4);
  // @rewrite-end

  unsigned char dst_f[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  unsigned char src_f[8] = {9, 9, 9, 9, 9, 9, 9, 9};
  int n = get_n();
  // @rewrite-begin
  memcpy(dst_f, src_f, n);
  // @rewrite-end

  unsigned char dst_g[4] = {0, 0, 0, 0};
  unsigned char *src_g = malloc(4);
  memset(src_g, 5, 4);
  // @rewrite-begin
  memcpy(dst_g, src_g, 4);
  // @rewrite-end
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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{__v[0-9]+}}: *mut u8 = dst_a.as_mut_ptr() as *mut u8;
// LOWERING-DAG: let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG: let {{__v[0-9]+}}: *mut u8 = src_a.as_mut_ptr() as *mut u8;
// LOWERING-DAG: let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG: let {{__v[0-9]+}}: u64 = 4;
// LOWERING-DAG: let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-DAG:     memcpy(
// LOWERING-DAG:         {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-DAG:         {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-DAG:         {{__v[0-9]+}} as usize,
// LOWERING-DAG:     )
// LOWERING-DAG: };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: dst_a.as_mut_ptr() as *mut u8;
// REWRITES-DAG: src_a.as_mut_ptr() as *mut u8;
// REWRITES-DAG: unsafe { dst_a[(0usize..4usize)].copy_from_slice(&src_a[(0usize..4usize)]) };
// REWRITES-DAG: dst_b.as_mut_ptr() as *mut u8;
// REWRITES-DAG: src_b.as_mut_ptr() as *mut u8;
// REWRITES-DAG: unsafe { dst_b[(0usize..5usize)].copy_from_slice(&src_b[(0usize..5usize)]) };
// REWRITES-DAG: self_c.as_mut_ptr() as *mut u8;
// REWRITES-DAG: self_c.as_mut_ptr() as *mut u8;
// REWRITES-DAG: unsafe { self_c.copy_within(0usize..8usize, 0usize) };
// REWRITES-DAG: dst_d.as_mut_ptr() as *mut u8;
// REWRITES-DAG: unsafe { dst_d[(0usize..5usize)].fill((65 as i32) as u8) };
// REWRITES-DAG: let {{__v[0-9]+}}: *mut u8 = dst_e.as_mut_ptr() as *mut u8;
// REWRITES-DAG: let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-DAG: let {{__v[0-9]+}}: *mut u8 = src_e.as_mut_ptr() as *mut u8;
// REWRITES-DAG: let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(2) };
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     std::ptr::copy_nonoverlapping({{__v[0-9]+}} as *const u8, {{__v[0-9]+}} as *mut u8, (4 as u64) as usize)
// REWRITES-DAG: };
// REWRITES-DAG: let {{__v[0-9]+}}: *mut u8 = dst_f.as_mut_ptr() as *mut u8;
// REWRITES-DAG: let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-DAG: let {{__v[0-9]+}}: *mut u8 = src_f.as_mut_ptr() as *mut u8;
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     std::ptr::copy_nonoverlapping({{__v[0-9]+}} as *const u8, {{__v[0-9]+}} as *mut u8, ({{__v[0-9]+}} as u64) as usize)
// REWRITES-DAG: };
// REWRITES-DAG: let {{__v[0-9]+}}: *mut u8 = dst_g.as_mut_ptr() as *mut u8;
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     std::ptr::copy_nonoverlapping({{__v[0-9]+}} as *const u8, {{__v[0-9]+}} as *mut u8, (4 as u64) as usize)
// REWRITES-DAG: };
// SLATE-FILECHECK-END rewrites
