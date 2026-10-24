#include <stdio.h>

// @check-fn-begin
static int load_at(const int *p, unsigned i) { return *(p + i); }
// @check-fn-end

// @check-fn-begin
static int load_byte(const unsigned char *p, unsigned long n) { return p[n / 8]; }
// @check-fn-end

// @check-fn-begin
static const int *back(const int *end, unsigned n) { return end - n; }
// @check-fn-end

// @check-fn-begin
static int signed_idx(const int *p, int i) { return *(p + i); }
// @check-fn-end

// @check-fn-begin
static int signed_back(const int *p, int i) { return *(p - i); }
// @check-fn-end

// @check-fn-begin
static const char *constant_offsets(const char *s) { return s[2] ? s + 8 : s - 3; }
// @check-fn-end

int main(void) {
  int a[4] = {1, 2, 3, 4};
  unsigned char b[4] = {9, 8, 7, 6};
  printf("%d %d %d %d %d\n", load_at(a, 2), load_byte(b, 16), *back(a + 3, 1),
         signed_idx(a + 1, -1), signed_back(a + 1, -2));
  char buf[16] = {0};
  printf("%d\n", constant_offsets(buf + 4) == buf + 1);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn load_at(p: *const i32, i: u32) -> i32 {
// CHECK-NEXT:     unsafe { *p.add(i as usize) }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn load_byte(p: *const u8, n: u64) -> i32 {
// CHECK-NEXT:     ((unsafe { *p.add((n / 8) as usize) }) as u32) as i32
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn back(end: *const i32, n: u32) -> *const i32 {
// CHECK-NEXT:     unsafe { end.sub(n as usize) }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn signed_idx(p: *const i32, i: i32) -> i32 {
// CHECK-NEXT:     unsafe { *p.offset(i as isize) }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn signed_back(p: *const i32, i: i32) -> i32 {
// CHECK-NEXT:     unsafe { *p.offset(-(i as isize)) }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn constant_offsets(s: *const i8) -> *const i8 {
// CHECK-NEXT:     if (unsafe { *s.add(2) }) != 0 { unsafe { s.add(8) } } else { unsafe { s.sub(3) } }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
