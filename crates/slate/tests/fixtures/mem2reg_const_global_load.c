#include <stdio.h>

struct Byte {
  signed char value;
};

// @lowering-fn-begin
// @rewrite-fn-begin
struct Byte make_byte(void) {
  struct Byte result = {7};
  return result;
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
int initialize_chars(void) {
  char text[4] = "abc";
  return 5;
}
// @rewrite-fn-end
// @lowering-fn-end

int main(void) {
  struct Byte byte = make_byte();
  printf("%d\n", byte.value + initialize_chars());
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn make_byte() -> Byte {
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: Byte = Byte { value: 7 };
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: fn initialize_chars() -> i32 {
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: [i8; 4] = [97, 98, 99, 0];
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: [u8; 4] = [97, 98, 99, 0];
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn make_byte() -> Byte {
// COMMON-REWRITES-DAG:     return Byte { value: 7 };
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: fn initialize_chars() -> i32 {
// COMMON-REWRITES-DAG:     5
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
