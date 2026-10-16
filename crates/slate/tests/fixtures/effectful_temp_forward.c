#include <stdio.h>

int add(int a, int b) { return a + b; }
int side_effect(void) {
  printf("effect\n");
  return 1;
}

// @check-fn-begin
int safe_forward(int a, int b, int c, int d) {
  int r    = add(a, b);
  int kept = c + d;
  printf("%d\n", r);
  return kept + kept;
}
// @check-fn-end

// @check-fn-begin
void blocked_forward(int a, int b) {
  int r = add(a, b);
  printf("%d %d\n", r, side_effect());
}
// @check-fn-end

int main(void) {
  printf("%d\n", safe_forward(2, 3, 4, 5));
  blocked_forward(6, 7);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn safe_forward(a: i32, b: i32, c: i32, d: i32) -> i32 {
// CHECK-NEXT:     let r: i32 = add(a, b);
// CHECK-NEXT:     let kept: i32 = c + d;
// CHECK-NEXT:     println!("{}", r);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     kept + kept
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn blocked_forward(a: i32, b: i32) {
// CHECK-NEXT:     let r: i32 = add(a, b);
// CHECK-NEXT:     println!("{} {}", r, side_effect());
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK: {{^}}}
// SLATE-FILECHECK-END
