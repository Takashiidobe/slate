#include <stdio.h>

// @check-fn-begin
static int forever(int a) { for (;;) { if (a > 9) break; a++; } return a; }
// @check-fn-end

// @check-fn-begin
static int while_one(int a) { while (1) { if (a > 20) break; a += 2; } return a; }
// @check-fn-end

// @check-fn-begin
static int braced(int a) { for (;;) { if (a > 30) { break; } a += 3; } return a; }
// @check-fn-end

// @check-fn-begin
static int do_head(int a) { do { if (a > 40) break; a += 4; } while (a < 45); return a; }
// @check-fn-end

// @check-fn-begin
static int goto_loop(int i, int limit) {
top:
  if (i >= limit) goto done;
  i++;
  goto top;
done:
  return i;
}
// @check-fn-end

// @check-fn-begin
static int scoped(int a) { for (;;) { int b = a * 2; if (b > 50) break; a += b; } return a; }
// @check-fn-end

int main(void) {
  printf("%d %d %d %d %d %d\n", forever(1), while_one(1), braced(1), do_head(1), goto_loop(0, 5), scoped(1));
  printf("%d %d\n", do_head(41), goto_loop(7, 5));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn forever(mut a: i32) -> i32 {
// CHECK-NEXT:     while !(a > 9) {
// CHECK-NEXT:         a += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     a
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn while_one(mut a: i32) -> i32 {
// CHECK-NEXT:     while !(a > 20) {
// CHECK-NEXT:         a += 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     a
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn braced(mut a: i32) -> i32 {
// CHECK-NEXT:     while !(a > 30) {
// CHECK-NEXT:         a += 3;
// CHECK-NEXT:     }
// CHECK-NEXT:     a
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn do_head(mut a: i32) -> i32 {
// CHECK-NEXT:     while !(a > 40) {
// CHECK-NEXT:         a += 4;
// CHECK-NEXT:         if a >= 45 {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     a
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn goto_loop(mut i: i32, limit: i32) -> i32 {
// CHECK-NEXT:     while !(i >= limit) {
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     i
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn scoped(mut a: i32) -> i32 {
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let b: i32 = a * 2;
// CHECK-NEXT:         if b > 50 {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         a += b;
// CHECK-NEXT:     }
// CHECK-NEXT:     a
// CHECK: {{^}}}
// SLATE-FILECHECK-END
