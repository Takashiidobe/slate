#include <stdio.h>

// @check-fn-begin
static int a(int n) { int s = 0; for (;;) { if (n > 0) { s += n; n--; } else { break; } } return s; }
// @check-fn-end

// @check-fn-begin
static int b(int n) { int s = 0; for (;;) { if (n > 0 && s < 100) { s += n; n--; } else break; } return s; }
// @check-fn-end

// @check-fn-begin
static int c(int n) { int s = 0; for (;;) { if (n > 0) { s += n; n--; if (s > 50) continue; s++; } else { break; } } return s; }
// @check-fn-end

// @check-fn-begin
static int d(int n) { int s = 0; for (;;) { if (n > 0) { s += n; n--; } else { s += 100; break; } } return s; }
// @check-fn-end

// @check-fn-begin
static int e(int n) { int s = 0; for (;;) { if (n > 0) { s += n; if (s > 9) break; n--; } else { break; } } return s; }
// @check-fn-end

int main(void) {
  printf("%d %d %d %d %d\n", a(5), b(5), c(3), d(2), e(9));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn a(mut n: i32) -> i32 {
// CHECK-NEXT:     let mut s: i32 = 0;
// CHECK-NEXT:     while n > 0 {
// CHECK-NEXT:         s += n;
// CHECK-NEXT:         n -= 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     s
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn b(mut n: i32) -> i32 {
// CHECK-NEXT:     let mut s: i32 = 0;
// CHECK-NEXT:     while n > 0 && s < 100 {
// CHECK-NEXT:         s += n;
// CHECK-NEXT:         n -= 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     s
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn c(mut n: i32) -> i32 {
// CHECK-NEXT:     let mut s: i32 = 0;
// CHECK-NEXT:     while n > 0 {
// CHECK-NEXT:         s += n;
// CHECK-NEXT:         n -= 1;
// CHECK-NEXT:         if s <= 50 {
// CHECK-NEXT:             s += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     s
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn d(mut n: i32) -> i32 {
// CHECK-NEXT:     let mut s: i32 = 0;
// CHECK-NEXT:     loop {
// CHECK-NEXT:         if n > 0 {
// CHECK-NEXT:             s += n;
// CHECK-NEXT:             n -= 1;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             s += 100;
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     s
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn e(mut n: i32) -> i32 {
// CHECK-NEXT:     let mut s: i32 = 0;
// CHECK-NEXT:     while n > 0 {
// CHECK-NEXT:         s += n;
// CHECK-NEXT:         if s > 9 {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         n -= 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     s
// CHECK: {{^}}}
// SLATE-FILECHECK-END
