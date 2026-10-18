#include <stdio.h>

static unsigned limit = 4;

// @check-fn-begin
static int sum(const int *a, int n) {
  int s = 0;
  for (int i = 0; i < n; i++) s += a[i];
  return s;
}
// @check-fn-end

// @check-fn-begin
static int skip_and_stop(int n) {
  int t = 0;
  for (int j = 0; j < n; ++j) {
    if (j == 7) break;
    if (j % 2) continue;
    t += j;
  }
  return t;
}
// @check-fn-end

// @check-fn-begin
static int triangle(int n) {
  int t = 0;
  for (int j = 0; j < n; j++)
    for (int k = 0; k < j; k++) t += k;
  return t;
}
// @check-fn-end

// @check-fn-begin
static int c89_style(int n) {
  int i;
  int t = 0;
  for (i = 2; i < n; i++) t += i * i;
  return t;
}
// @check-fn-end

// @check-fn-begin
static unsigned unsigned_sum(unsigned n) {
  unsigned t = 0;
  for (unsigned i = 0; i < n; i++) t += i;
  return t;
}
// @check-fn-end

// @check-fn-begin
static int shrinking_bound(int n) {
  int t = 0;
  for (int i = 0; i < n; i++) {
    if (i == 2) n--;
    t += i;
  }
  return t;
}
// @check-fn-end

// @check-fn-begin
static int bound_through_pointer(int n) {
  int *p = &n;
  int  t = 0;
  for (int i = 0; i < n; i++) {
    t += i;
    if (i == 1) *p = 3;
  }
  return t;
}
// @check-fn-end

// @check-fn-begin
static int find(const int *a, int n, int x) {
  int i;
  for (i = 0; i < n; i++)
    if (a[i] == x) break;
  return i;
}
// @check-fn-end

// @check-fn-begin
static int stride(int n) {
  int t = 0;
  for (int i = 0; i < n; i++) {
    t += i;
    if (i % 3 == 0) i++;
  }
  return t;
}
// @check-fn-end

// @check-fn-begin
static unsigned global_bound(void) {
  unsigned t = 0;
  for (unsigned i = 0; i < limit; i++) {
    t += i;
    if (i == 1) limit = 6;
  }
  return t;
}
// @check-fn-end

int main(void) {
  int a[5] = {1, 2, 3, 4, 5};
  printf("%d %d %d %d %u\n", sum(a, 5), skip_and_stop(12), triangle(6), c89_style(6), unsigned_sum(7));
  printf("%d %d %d %d %u\n", shrinking_bound(6), bound_through_pointer(8), find(a, 5, 4), stride(10), global_bound());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn sum(a: *const i32, n: i32) -> i32 {
// CHECK-NEXT:     let mut s: i32 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         s += unsafe { *a.offset(i as isize) };
// CHECK-NEXT:     }
// CHECK-NEXT:     s
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn skip_and_stop(n: i32) -> i32 {
// CHECK-NEXT:     let mut t: i32 = 0;
// CHECK-NEXT:     for j in 0..n {
// CHECK-NEXT:         if j == 7 {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         if j % 2 == 0 {
// CHECK-NEXT:             t += j;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     t
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn triangle(n: i32) -> i32 {
// CHECK-NEXT:     let mut t: i32 = 0;
// CHECK-NEXT:     for j in 0..n {
// CHECK-NEXT:         for k in 0..j {
// CHECK-NEXT:             t += k;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     t
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn c89_style(n: i32) -> i32 {
// CHECK-NEXT:     let mut t: i32 = 0;
// CHECK-NEXT:     for i in 2..n {
// CHECK-NEXT:         t += i * i;
// CHECK-NEXT:     }
// CHECK-NEXT:     t
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn unsigned_sum(n: u32) -> u32 {
// CHECK-NEXT:     let mut t: u32 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         t = t.wrapping_add(i);
// CHECK-NEXT:     }
// CHECK-NEXT:     t
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn shrinking_bound(mut n: i32) -> i32 {
// CHECK-NEXT:     let mut t: i32 = 0;
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while i < n {
// CHECK-NEXT:         if i == 2 {
// CHECK-NEXT:             n -= 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         t += i;
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     t
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn bound_through_pointer(mut n: i32) -> i32 {
// CHECK-NEXT:     let p: *mut i32 = std::ptr::addr_of_mut!(n);
// CHECK-NEXT:     let mut t: i32 = 0;
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while i < n {
// CHECK-NEXT:         t += i;
// CHECK-NEXT:         if i == 1 {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *p = 3;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     t
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn find(a: *const i32, n: i32, x: i32) -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while i < n {
// CHECK-NEXT:         if (unsafe { *a.offset(i as isize) }) == x {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     i
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn stride(n: i32) -> i32 {
// CHECK-NEXT:     let mut t: i32 = 0;
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while i < n {
// CHECK-NEXT:         t += i;
// CHECK-NEXT:         if i % 3 == 0 {
// CHECK-NEXT:             i += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     t
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn global_bound() -> u32 {
// CHECK-NEXT:     let mut t: u32 = 0;
// CHECK-NEXT:     let mut i: u32 = 0;
// CHECK-NEXT:     while i < unsafe { limit } {
// CHECK-NEXT:         t = t.wrapping_add(i);
// CHECK-NEXT:         if i == 1 {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 limit = 6;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         i = i.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT:     t
// CHECK: {{^}}}
// SLATE-FILECHECK-END
