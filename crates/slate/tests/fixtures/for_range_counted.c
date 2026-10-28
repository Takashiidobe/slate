#include <stdio.h>

static int calls;

static int next_start(int base) {
  calls++;
  return base - 2;
}

// @check-fn-begin
static int empty_and_negative(int lo, int hi) {
  int t = 0;
  for (int i = hi; i < lo; i++) t += 100;
  for (int i = -3; i < lo; i++) t += i;
  return t;
}
// @check-fn-end

// @check-fn-begin
static void print_indices(void) {
  for (unsigned long k = 0; k <= 3; k++) printf("%lu ", k);
  for (unsigned j = 2; 5 > j; ++j) printf("%u ", j);
  printf("\n");
}
// @check-fn-end

// @check-fn-begin
static int skip_odd(int n) {
  int t = 0;
  for (int i = 0; i < n; i++) {
    if (i & 1) continue;
    if (i > 10) break;
    t += i;
  }
  return t;
}
// @check-fn-end

// @check-fn-begin
static int switch_continue(int n) {
  int t = 0;
  for (int i = 0; i < n; i++) {
    switch (i % 3) {
    case 0:
      continue;
    case 1:
      t += 10;
      break;
    default:
      t += 1;
    }
    t += 100;
  }
  return t;
}
// @check-fn-end

// @check-fn-begin
static int nested_continue(int n) {
  int t = 0;
  for (int i = 0; i < n; i++) {
    int j = 0;
    while (j < i) {
      j++;
      if (j == 2) continue;
      if (j == 4) break;
      t += j;
    }
    for (int i = 0; i < 2; i++) {
      if (i == 1) continue;
      t += 1000;
    }
    if (i == 3) continue;
    t += i;
  }
  return t;
}
// @check-fn-end

// @check-fn-begin
static int call_start(int n) {
  int t = 0;
  for (int i = next_start(n); i < n; i = i + 1) t += i;
  return t;
}
// @check-fn-end

// @check-fn-begin
static int shadow_param(const int *i, int n) {
  int t = *i;
  for (int i = 0; i < n; i++) t += 2 * i;
  return t;
}
// @check-fn-end

int main(void) {
  printf("%d %d\n", empty_and_negative(4, 9), empty_and_negative(-5, 0));
  print_indices();
  printf("%d %d\n", skip_odd(20), switch_continue(7));
  printf("%d\n", nested_continue(7));
  printf("%d %d\n", call_start(10), calls);
  int base = 40;
  printf("%d\n", shadow_param(&base, 4));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn empty_and_negative(lo: i32, hi: i32) -> i32 {
// CHECK-NEXT:     let mut t: i32 = 0;
// CHECK-NEXT:     for i in hi..lo {
// CHECK-NEXT:         t += 100;
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in -3..lo {
// CHECK-NEXT:         t += i;
// CHECK-NEXT:     }
// CHECK-NEXT:     t
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn print_indices() {
// CHECK-NEXT:     for k in 0..4u64 {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"%lu ".as_ptr(), k) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     for j in 2u32..5u32 {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"%u ".as_ptr(), j) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn skip_odd(n: i32) -> i32 {
// CHECK-NEXT:     let mut t: i32 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         if i & 1 != 0 {
// CHECK-NEXT:             continue;
// CHECK-NEXT:         }
// CHECK-NEXT:         if i > 10 {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         t += i;
// CHECK-NEXT:     }
// CHECK-NEXT:     t
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn switch_continue(n: i32) -> i32 {
// CHECK-NEXT:     let mut t: i32 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         match i % 3 {
// CHECK-NEXT:             0 => {
// CHECK-NEXT:                 continue;
// CHECK-NEXT:             }
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 t += 10;
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 t += 1;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         t += 100;
// CHECK-NEXT:     }
// CHECK-NEXT:     t
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn nested_continue(n: i32) -> i32 {
// CHECK-NEXT:     let mut t: i32 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         let mut j: i32 = 0;
// CHECK-NEXT:         while j < i {
// CHECK-NEXT:             j += 1;
// CHECK-NEXT:             if j != 2 {
// CHECK-NEXT:                 if j == 4 {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 t += j;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         for i in 0i32..2i32 {
// CHECK-NEXT:             if i == 1 {
// CHECK-NEXT:                 continue;
// CHECK-NEXT:             }
// CHECK-NEXT:             t += 1000;
// CHECK-NEXT:         }
// CHECK-NEXT:         if i == 3 {
// CHECK-NEXT:             continue;
// CHECK-NEXT:         }
// CHECK-NEXT:         t += i;
// CHECK-NEXT:     }
// CHECK-NEXT:     t
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn call_start(n: i32) -> i32 {
// CHECK-NEXT:     let mut t: i32 = 0;
// CHECK-NEXT:     for i in next_start(n)..n {
// CHECK-NEXT:         t += i;
// CHECK-NEXT:     }
// CHECK-NEXT:     t
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn shadow_param(i: *const i32, n: i32) -> i32 {
// CHECK-NEXT:     let mut t: i32 = unsafe { *i };
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         t += 2 * i;
// CHECK-NEXT:     }
// CHECK-NEXT:     t
// CHECK: {{^}}}
// SLATE-FILECHECK-END
