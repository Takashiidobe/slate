#include <stdio.h>

// @check-fn-begin
static int contains(const int *a, int n, int key) {
  int found = 0;
  for (int i = 0; i < n; i++)
    if (a[i] == key)
      found = 1;
  if (found)
    return 10;
  return 20;
}
// @check-fn-end

// @check-fn-begin
static int ordered(int a, int b, int c) {
  int lo = a < b;
  int hi = b <= c;
  int both = lo && hi;
  if (!both)
    return -1;
  return 1;
}
// @check-fn-end

// @check-fn-begin
static int all_positive(const int *a, int n) {
  int ok = 1;
  for (int i = 0; i < n && ok; i++)
    ok = a[i] > 0;
  return ok ? 3 : 4;
}
// @check-fn-end

// @check-fn-begin
static int same(int a, int b) {
  int eq = a == b;
  return eq;
}
// @check-fn-end

// @check-fn-begin
static int count_even(const int *a, int n) {
  int total = 0;
  for (int i = 0; i < n; i++) {
    int even = a[i] % 2 == 0;
    total += even;
  }
  return total;
}
// @check-fn-end

// @check-fn-begin
static int not_zero_one(int a) {
  int state = a > 0;
  if (a > 100)
    state = 2;
  return state;
}
// @check-fn-end

// @check-fn-begin
static int mixed_uses(int a, int b) {
  int lt = a < b;
  if (lt)
    return 7;
  if (!lt && a > 5)
    return 8;
  return lt ? 1 : lt + 1;
}
// @check-fn-end

static int parity(int a) { return a & 1; }

// @check-fn-begin
static int call_defined(int a) {
  int odd = parity(a);
  if (odd)
    return 8;
  return 9;
}
// @check-fn-end

static void clear(int *p) { *p = 0; }

// @check-fn-begin
static int address_taken(int a, int b) {
  int lt = a < b;
  if (a == 7)
    clear(&lt);
  return lt ? 5 : 6;
}
// @check-fn-end

int main(void) {
  int v[4] = {2, 4, 5, 8};
  int neg[3] = {1, -2, 3};
  printf("%d %d\n", contains(v, 4, 5), contains(v, 4, 9));
  printf("%d %d\n", ordered(1, 2, 3), ordered(3, 2, 1));
  printf("%d %d\n", all_positive(v, 4), all_positive(neg, 3));
  printf("%d %d\n", same(4, 4), same(4, 5));
  printf("%d\n", count_even(v, 4));
  printf("%d %d %d\n", not_zero_one(-1), not_zero_one(5), not_zero_one(200));
  printf("%d %d %d\n", mixed_uses(1, 2), mixed_uses(2, 1), mixed_uses(9, 3));
  printf("%d %d\n", call_defined(3), call_defined(4));
  printf("%d %d\n", address_taken(1, 2), address_taken(7, 8));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn contains(a: &[i32], key: i32) -> i32 {
// CHECK-NEXT:     let n: i32 = a.len() as i32;
// CHECK-NEXT:     let mut found: bool = false;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         if a[i as usize] == key {
// CHECK-NEXT:             found = true;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if found {
// CHECK-NEXT:         return 10;
// CHECK-NEXT:     }
// CHECK-NEXT:     20
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn ordered(a: i32, b: i32, c: i32) -> i32 {
// CHECK-NEXT:     let lo: bool = a < b;
// CHECK-NEXT:     let hi: bool = b <= c;
// CHECK-NEXT:     let both: bool = lo && hi;
// CHECK-NEXT:     if !both {
// CHECK-NEXT:         return -1;
// CHECK-NEXT:     }
// CHECK-NEXT:     1
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn all_positive(a: &[i32]) -> i32 {
// CHECK-NEXT:     let n: i32 = a.len() as i32;
// CHECK-NEXT:     let mut ok: bool = true;
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while i < n && ok {
// CHECK-NEXT:         ok = a[i as usize] > 0;
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     if ok { 3 } else { 4 }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn same(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     let eq: i32 = (a == b) as i32;
// CHECK-NEXT:     eq
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn count_even(a: &[i32]) -> i32 {
// CHECK-NEXT:     let n: i32 = a.len() as i32;
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         let even: i32 = (a[i as usize] % 2 == 0) as i32;
// CHECK-NEXT:         total += even;
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn not_zero_one(a: i32) -> i32 {
// CHECK-NEXT:     let mut state: i32 = (a > 0) as i32;
// CHECK-NEXT:     if a > 100 {
// CHECK-NEXT:         state = 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     state
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn mixed_uses(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     let lt: bool = a < b;
// CHECK-NEXT:     if lt {
// CHECK-NEXT:         return 7;
// CHECK-NEXT:     }
// CHECK-NEXT:     if !lt && a > 5 {
// CHECK-NEXT:         return 8;
// CHECK-NEXT:     }
// CHECK-NEXT:     if lt { 1 } else { (lt as i32) + 1 }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn call_defined(a: i32) -> i32 {
// CHECK-NEXT:     let odd: i32 = parity(a);
// CHECK-NEXT:     if odd != 0 {
// CHECK-NEXT:         return 8;
// CHECK-NEXT:     }
// CHECK-NEXT:     9
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn address_taken(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     let mut lt: i32 = (a < b) as i32;
// CHECK-NEXT:     if a == 7 {
// CHECK-NEXT:         clear(&mut lt);
// CHECK-NEXT:     }
// CHECK-NEXT:     if lt != 0 { 5 } else { 6 }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
