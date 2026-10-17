#include <stdio.h>

static int interpret(const unsigned char *code, int length) {
  static const int offsets[] = {&&add - &&dispatch, &&double_it - &&dispatch,
                                &&subtract - &&dispatch};
  int              index     = 0;
  int              value     = 1;

dispatch:
  if (index == length)
    return value;
  goto *(&&dispatch + offsets[code[index++]]);

add:
  value += 3;
  goto dispatch;
double_it:
  value *= 2;
  goto dispatch;
subtract:
  value -= 5;
  goto dispatch;
}

// @check-fn-begin
int main(void) {
  const unsigned char code[] = {0, 1, 2, 1};
  printf("%d\n", interpret(code, 4));
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut code: [u8; 4] = [0, 1, 2, 1];
// CHECK-NEXT:     println!("{}", interpret(code.as_ptr() as * const u8, 4 as i32));
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
