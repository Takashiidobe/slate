#include <stdio.h>

static int counts[4] = {1, 2, 3, 4};
static const int weights[4] = {5, 6, 7, 8};
static int scratch[4] = {1, 2, 3, 4};
static int history[4] = {9, 8, 7, 6};
static int *history_tail = &history[3];
static int ring[4] = {2, 4, 6, 8};
static int tally[4] = {1, 1, 1, 1};
static int calls;
static int pair[4] = {3, 3, 3, 3};

__attribute__((noinline)) static void poke(void) { counts[0] += 100; }
__attribute__((noinline)) static void note(void) { calls++; }
__attribute__((noinline)) static void tick(void) { *history_tail += 1000; }
__attribute__((noinline)) static void spin(void) { ring[1] += 10; }
__attribute__((noinline)) static int bump_tally(void) {
  tally[2] += 50;
  return 1;
}

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

// @check-fn-begin
static void scale_untouched(int *p, int n) {
  for (int i = 0; i < n; i++) {
    p[i] *= 2;
    note();
  }
}
// @check-fn-end

// @check-fn-begin
static int sum_leaked(const int *p, int n) {
  int before = p[3];
  tick();
  return p[3] - before + n;
}
// @check-fn-end

// @check-fn-begin
static int sum_indirect(const int *p, int n, void (*hook)(void)) {
  int before = p[1];
  hook();
  return p[1] - before + n;
}
// @check-fn-end

// @check-fn-begin
static int sum_after(const int *p, int n, int extra) {
  int total = extra;
  for (int i = 0; i < n; i++)
    total += p[i];
  return total;
}
// @check-fn-end

// @check-fn-begin
static int sum_alias(const int *p, int n, int *q) {
  int before = p[0];
  q[0] = 9;
  return p[0] - before + n;
}
// @check-fn-end

int main(void) {
  int *alias = pair;
  printf("%d\n", sum_mutable(counts, 4));
  printf("%d\n", sum_const(weights, 4));
  scale_untouched(scratch, 4);
  scratch[0] += 1;
  printf("%d %d %d %d %d\n", scratch[0], scratch[1], scratch[2], scratch[3], calls);
  printf("%d\n", sum_leaked(history, 4));
  printf("%d\n", sum_indirect(ring, 4, spin));
  printf("%d\n", sum_after(tally, 4, bump_tally()));
  printf("%d\n", sum_alias(pair, 4, alias));
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
// CHECK-LABEL: {{^}}fn scale_untouched(p: &mut [i32]) {
// CHECK-NEXT:     let n: i32 = p.len() as i32;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         p[i as usize] *= 2;
// CHECK-NEXT:         note();
// CHECK-NEXT:     }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn sum_leaked(p: *const i32, n: i32) -> i32 {
// CHECK-NEXT:     let before: i32 = unsafe { *p.add(3) };
// CHECK-NEXT:     tick();
// CHECK-NEXT:     (unsafe { *p.add(3) }) - before + n
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn sum_indirect(
// CHECK-NEXT:     p: *const i32,
// CHECK-NEXT:     n: i32,
// CHECK-NEXT:     hook: Option<unsafe extern "C-unwind" fn()>,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     let before: i32 = unsafe { *p.add(1) };
// CHECK-NEXT:     unsafe { hook.unwrap()() };
// CHECK-NEXT:     (unsafe { *p.add(1) }) - before + n
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn sum_after(p: *const i32, n: i32, mut total: i32) -> i32 {
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         total += unsafe { *p.offset(i as isize) };
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn sum_alias(p: *const i32, n: i32, q: *mut i32) -> i32 {
// CHECK-NEXT:     let before: i32 = unsafe { *p.add(0) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *q.add(0) = 9;
// CHECK-NEXT:     }
// CHECK-NEXT:     (unsafe { *p.add(0) }) - before + n
// CHECK: {{^}}}
// SLATE-FILECHECK-END
