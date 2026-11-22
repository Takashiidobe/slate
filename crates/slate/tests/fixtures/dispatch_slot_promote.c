#include <stdio.h>

struct Pair {
  int a;
  int b;
};

static void bump(int *p) { *p += 10; }

// @check-fn-begin
static int counted(int n) {
  int i = 0;
  int total = 0;
again:
  total += i;
  i++;
  if (i < n) goto again;
  return total;
}
// @check-fn-end

// @check-fn-begin
static int escaped(int n) {
  int x = n;
  int k = 0;
top:
  bump(&x);
  k++;
  if (k < 3) goto top;
  return x;
}
// @check-fn-end

// @check-fn-begin
static int record(int n) {
  struct Pair p = {1, 2};
  int k = 0;
loop:
  p.a += p.b;
  if (++k < n) goto loop;
  return p.a;
}
// @check-fn-end

// @check-fn-begin
static int aligned(int n) {
  _Alignas(32) int v = 1;
  int k = 0;
loop:
  v *= 2;
  if (++k < n) goto loop;
  return v;
}
// @check-fn-end

int main(void) {
  printf("%d %d %d %d\n", counted(5), escaped(1), record(4), aligned(3));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn counted(n: i32) -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     loop {
// CHECK-NEXT:         total += i;
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:         if i >= n {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn escaped(n: i32) -> i32 {
// CHECK-NEXT:     let mut x: i32 = n;
// CHECK-NEXT:     let mut k: i32 = 0;
// CHECK-NEXT:     loop {
// CHECK-NEXT:         bump(&mut x);
// CHECK-NEXT:         k += 1;
// CHECK-NEXT:         if k >= 3 {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn record(n: i32) -> i32 {
// CHECK-NEXT:     let mut p: Pair = Pair { a: 1, b: 2 };
// CHECK-NEXT:     let mut k: i32 = 0;
// CHECK-NEXT:     loop {
// CHECK-NEXT:         p.a += p.b;
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = k + 1;
// CHECK-NEXT:         k = [[V_0]];
// CHECK-NEXT:         if [[V_0]] >= n {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     p.a
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn aligned(n: i32) -> i32 {
// CHECK-NEXT:     let mut v: __SlateAlign32<i32> = __SlateAlign32(1i32);
// CHECK-NEXT:     let mut k: i32 = 0;
// CHECK-NEXT:     loop {
// CHECK-NEXT:         v.0 *= 2;
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = k + 1;
// CHECK-NEXT:         k = [[V_0]];
// CHECK-NEXT:         if [[V_0]] >= n {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     v.0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
