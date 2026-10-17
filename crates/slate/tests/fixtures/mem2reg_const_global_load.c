#include <stdio.h>

struct Byte {
  signed char value;
};

// @check-fn-begin
struct Byte make_byte(void) {
  struct Byte result = {7};
  return result;
}
// @check-fn-end

// @check-fn-begin
int initialize_chars(void) {
  char text[4] = "abc";
  return 5;
}
// @check-fn-end

int main(void) {
  struct Byte byte = make_byte();
  printf("%d\n", byte.value + initialize_chars());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn make_byte() -> Byte {
// CHECK-NEXT:     let result: Byte = Byte { value: 7 };
// CHECK-NEXT:     return result;
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn initialize_chars() -> i32 {
// CHECK-NEXT:     5
// CHECK: {{^}}}
// SLATE-FILECHECK-END
