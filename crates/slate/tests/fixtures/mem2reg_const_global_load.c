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
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn make_byte() -> i8 {
// LOWERING-DAG: let mut coerce: Byte = Byte { value: 0 };
// LOWERING-DAG: let {{_v[0-9]+}}: Byte = Byte { value: 7 };
// LOWERING-DAG: coerce = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = std::ptr::addr_of_mut!(coerce) as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-DAG: return {{_v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn initialize_chars() -> i32 {
// LOWERING-DAG: let {{_v[0-9]+}}: [i8; 4] = [97, 98, 99, 0];
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 5;
// LOWERING-DAG: return {{_v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn make_byte() -> i8 {
// REWRITES-DAG: let mut coerce: Byte = Byte { value: 0 };
// REWRITES-DAG: coerce = Byte { value: 7 };
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = std::ptr::addr_of_mut!(coerce) as *mut i8;
// REWRITES-DAG: return unsafe { *{{_v[0-9]+}} };
// REWRITES-DAG: }
// REWRITES-DAG: fn initialize_chars() -> i32 {
// REWRITES-DAG: let {{_v[0-9]+}}: [i8; 4] = [97, 98, 99, 0];
// REWRITES-DAG: return 5;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
