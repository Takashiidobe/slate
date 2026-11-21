#include <stdio.h>

static int counts[4] = {1, 2, 3, 4};
static const int weights[4] = {5, 6, 7, 8};

__attribute__((noinline)) static void poke(void) { counts[0] += 100; }

// @check-fn-begin
static int sum_mutable(const int *p, int n) {
  int before = p[0];
  poke();
  int total = 0;
  for (int i = 0; i < n; i++)
    total += p[i];
  return total - before;
}
// @check-fn-end

// @check-fn-begin
static int sum_const(const int *p, int n) {
  int total = 0;
  for (int i = 0; i < n; i++)
    total += p[i];
  return total;
}
// @check-fn-end

int main(void) {
  printf("%d\n", sum_mutable(counts, 4));
  printf("%d\n", sum_const(weights, 4));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn sum_mutable(p: *const i32, n: i32) -> i32 {
// CHECK-NEXT:     let before: i32 = unsafe { *p.add(0) };
// CHECK-NEXT:     poke();
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         total += unsafe { *p.offset(i as isize) };
// CHECK-NEXT:     }
// CHECK-NEXT:     total - before
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn sum_const(p: &[i32]) -> i32 {
// CHECK-NEXT:     let n: i32 = p.len() as i32;
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         total += p[i as usize];
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK: {{^}}}
// SLATE-FILECHECK-END
