#include <stdbool.h>
#include <stdio.h>

// @check-fn-begin
static bool big(int x) { return x > 3 ? true : false; }
// @check-fn-end

// @check-fn-begin
static bool small(int x) { return x > 3 ? false : true; }
// @check-fn-end

// @check-fn-begin
static bool both(bool c, int y) { return c ? y > 0 : false; }
// @check-fn-end

// @check-fn-begin
static bool either(bool c, int y) { return c ? true : y < 0; }
// @check-fn-end

// @check-fn-begin
static bool flag_or(int x, bool flag) {
  bool r = x == 2 ? true : flag;
  return r;
}
// @check-fn-end

// @check-fn-begin
static int counted(int x) { return x > 3 ? 7 : 0; }
// @check-fn-end

int main(void) {
  int total = 0;
  for (int i = 0; i < 6; i++) {
    total = total * 2 + big(i);
    total = total * 2 + small(i);
    total = total * 2 + both(i & 1, i - 3);
    total = total * 2 + either(i & 2, i - 4);
    total = total * 2 + flag_or(i, i == 5);
    total += counted(i);
    total %= 100003;
  }
  printf("%d\n", total);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn big(x: i32) -> bool {
// CHECK-NEXT:     x > 3
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn small(x: i32) -> bool {
// CHECK-NEXT:     x <= 3
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn both(c: bool, y: i32) -> bool {
// CHECK-NEXT:     c && y > 0
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn either(c: bool, y: i32) -> bool {
// CHECK-NEXT:     c || y < 0
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn flag_or(x: i32, flag: bool) -> bool {
// CHECK-NEXT:     let r: bool = x == 2 || flag;
// CHECK-NEXT:     r
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn counted(x: i32) -> i32 {
// CHECK-NEXT:     if x > 3 { 7 } else { 0 }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
