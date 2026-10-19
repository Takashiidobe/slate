#include <stdio.h>

static int int_score(int value) { return value + 10; }
static int long_score(long value) { return (int)value + 20; }
static int pointer_score(const int *value) { return *value + 30; }

#define SCORE(value)                                                           \
  _Generic((value),                                                            \
      int: int_score,                                                          \
      long: long_score,                                                        \
      const int *: pointer_score)(value)

// @check-fn-begin
int main(void) {
  const int value   = 7;
  int       array[] = {5, 6};
  int       first   = SCORE(value);
  int       second  = SCORE(8L);
  int       third   = SCORE((const int *)array);
  printf("%d %d %d\n", first, second, third);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let value: i32 = 7;
// CHECK-NEXT:     let mut array: [i32; 2] = [5, 6];
// CHECK-NEXT:     let first: i32 = int_score(value);
// CHECK-NEXT:     let second: i32 = long_score(8i64);
// CHECK-NEXT:     let third: i32 = pointer_score(array.as_mut_ptr() as *const i32);
// CHECK-NEXT:     println!("{} {} {}", first, second, third);
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
