#include <stdio.h>
#include <string.h>

static int sum(const char *bytes, int n) {
  int total = 0;
  for (int i = 0; i < n; i++)
    total = total * 3 + bytes[i];
  return total;
}

// @check-fn-begin
static int alias_fill(void) {
  char  buf[8];
  char *p = buf;
  memset(p, 7, 8);
  return sum(buf, 8);
}
// @check-fn-end

// @check-fn-begin
static int offset_copy(void) {
  char  src[6] = {1, 2, 3, 4, 5, 6};
  char  dst[8] = {0};
  char *tail = dst + 2;
  memcpy(tail, src, 6);
  return sum(dst, 8);
}
// @check-fn-end

// @check-fn-begin
static int same_object_move(void) {
  char  buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  char *shifted = buf + 1;
  memmove(shifted, buf, 6);
  memcpy(buf + 4, buf, 2);
  return sum(buf, 8);
}
// @check-fn-end

// @check-fn-begin
static int runtime_length(int n) {
  char buf[16] = {0};
  memset(buf + 1, 9, n);
  return sum(buf, 16);
}
// @check-fn-end

// @check-fn-begin
static int compare(void) {
  unsigned char  a[4] = {1, 2, 3, 200};
  unsigned char  b[4] = {1, 2, 9, 4};
  unsigned char *pa = a;
  return memcmp(pa, b, 4) < 0;
}
// @check-fn-end

// @check-fn-begin
static int shadowed(void) {
  char  buf[4] = {1, 2, 3, 4};
  char *p = buf;
  {
    char buf[4] = {5, 6, 7, 8};
    memset(p, 0, 2);
    return sum(buf, 4);
  }
}
// @check-fn-end

// @check-fn-begin
static int unknown(char *dst, const char *src) {
  memcpy(dst, src, 3);
  return sum(dst, 3);
}
// @check-fn-end

int main(void) {
  char out[3];
  printf("%d\n", alias_fill());
  printf("%d\n", offset_copy());
  printf("%d\n", same_object_move());
  printf("%d\n", runtime_length(5));
  printf("%d\n", compare());
  printf("%d\n", shadowed());
  printf("%d\n", unknown(out, "xyz"));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn alias_fill() -> i32 {
// CHECK-NEXT:     let mut buf: [i8; 8] = [0i8; 8];
// CHECK-NEXT:     buf[0usize..8usize].fill((7i32 as i32) as i8);
// CHECK-NEXT:     sum(buf.as_mut_ptr() as *const i8, 8i32)
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn offset_copy() -> i32 {
// CHECK-NEXT:     let mut src: [i8; 6] = [1, 2, 3, 4, 5, 6];
// CHECK-NEXT:     let mut dst: [i8; 8] = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [i8; 8] = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = 0i8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     dst[2usize..8usize].copy_from_slice(&src[0usize..6usize]);
// CHECK-NEXT:     sum(dst.as_mut_ptr() as *const i8, 8i32)
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn same_object_move() -> i32 {
// CHECK-NEXT:     let mut buf: [i8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// CHECK-NEXT:     buf.copy_within(0usize..6usize, 1usize);
// CHECK-NEXT:     buf.copy_within(0usize..2usize, 4usize);
// CHECK-NEXT:     sum(buf.as_mut_ptr() as *const i8, 8i32)
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn runtime_length(n: i32) -> i32 {
// CHECK-NEXT:     let mut buf: __SlateAlign16<[i8; 16]> = __SlateAlign16(
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_0:__t[0-9]+]]: [i8; 16] = unsafe { core::mem::zeroed() };
// CHECK-NEXT:             [[T_0]][0] = 0i8;
// CHECK-NEXT:             [[T_0]]
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     buf.0[1usize..(1usize + (((n as u64) as usize) as usize))].fill((9i32 as i32) as i8);
// CHECK-NEXT:     sum(buf.0.as_mut_ptr() as *const i8, 16i32)
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn compare() -> i32 {
// CHECK-NEXT:     let mut a: [u8; 4] = [1, 2, 3, 200];
// CHECK-NEXT:     let mut b: [u8; 4] = [1, 2, 9, 4];
// CHECK-NEXT:     ((a[0usize..4usize].cmp(&b[0usize..4usize]) as i32) < 0) as i32
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn shadowed() -> i32 {
// CHECK-NEXT:     let mut buf: [i8; 4] = [1, 2, 3, 4];
// CHECK-NEXT:     let p: *mut i8 = buf.as_mut_ptr();
// CHECK-NEXT:     let mut buf: [i8; 4] = [5, 6, 7, 8];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::write_bytes(
// CHECK-NEXT:             ((p as *mut ()) as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             (0i32 as i32) as u8,
// CHECK-NEXT:             (2u64 as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     sum(buf.as_mut_ptr() as *const i8, 4i32)
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn unknown(dst: &mut [i8], src: *const i8) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             ((src as *const ()) as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((dst.as_mut_ptr() as *mut ()) as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             (3u64 as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     sum(dst.as_mut_ptr() as *const i8, 3i32)
// CHECK: {{^}}}
// SLATE-FILECHECK-END
