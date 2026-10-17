#include <stdio.h>

struct Point {
  int x;
  long y;
};

// @check-fn-begin
static long widen(int n) {
  long t = 0;
  if (n > 3) t = n - 1;
  return t + 3;
}
// @check-fn-end

// @check-fn-begin
static unsigned long long shifts(int k) {
  unsigned long long big = 1ULL << 40;
  long long wide = (long long)1 << k;
  return big + (unsigned long long)wide;
}
// @check-fn-end

// @check-fn-begin
static unsigned wraps(unsigned u) {
  unsigned all = (unsigned)-1;
  return u + all + 2u;
}
// @check-fn-end

// @check-fn-begin
static int negatives(int n) {
  int lo = -5;
  if (n < -2) return -1;
  return n * -3 + lo;
}
// @check-fn-end

// @check-fn-begin
static double floats(int n) {
  double d = 2;
  float f = 1.5f;
  return d * n + f;
}
// @check-fn-end

// @check-fn-begin
static long aggregates(void) {
  struct Point p = {1, 2};
  short s[3] = {1, 2, 3};
  return p.x + p.y + s[2];
}
// @check-fn-end

// @check-fn-begin
static const char *text(int n) {
  return n ? "yes" : "no";
}
// @check-fn-end

int main(void) {
  printf("%ld %llu %u %d\n", widen(5), shifts(3), wraps(4), negatives(-4));
  printf("%f %ld %s %lld %d\n", floats(2), aggregates(), text(1), 5000000000LL, 7);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn widen(n: i32) -> i64 {
// CHECK-NEXT:     let mut t: i64 = 0;
// CHECK-NEXT:     if n > 3 {
// CHECK-NEXT:         t = (n - 1) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     t + 3
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn shifts(k: i32) -> u64 {
// CHECK-NEXT:     let big: u64 = (1 as u64) << (40 as i32);
// CHECK-NEXT:     let wide: i64 = (1 as i64) << k;
// CHECK-NEXT:     big.wrapping_add(wide as u64)
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn wraps(u: u32) -> u32 {
// CHECK-NEXT:     let all: u32 = -(1 as i32) as u32;
// CHECK-NEXT:     u.wrapping_add(all).wrapping_add(2)
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn negatives(n: i32) -> i32 {
// CHECK-NEXT:     let lo: i32 = -5;
// CHECK-NEXT:     if n < -2 {
// CHECK-NEXT:         return -1;
// CHECK-NEXT:     }
// CHECK-NEXT:     n * -3 + lo
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn floats(n: i32) -> f64 {
// CHECK-NEXT:     let d: f64 = (2 as i32) as f64;
// CHECK-NEXT:     let f: f32 = 1.5f32;
// CHECK-NEXT:     d * (n as f64) + (f as f64)
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn aggregates() -> i64 {
// CHECK-NEXT:     let p: Point = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: Point = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].x = 1 as i32;
// CHECK-NEXT:         [[T_0]].y = 2 as i64;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let s: [i16; 3] = [1, 2, 3];
// CHECK-NEXT:     (p.x as i64) + p.y + ((s[2] as i32) as i64)
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn text(n: i32) -> *const i8 {
// CHECK-NEXT:     (if n != 0 { c"yes".as_ptr() as *mut i8 } else { c"no".as_ptr() as *mut i8 })
// CHECK-NEXT:         as *const i8
// CHECK: {{^}}}
// SLATE-FILECHECK-END
