#include <stdarg.h>
#include <stdio.h>

// @lowering-fn-begin
// @rewrite-fn-begin
int consume(va_list arguments) { return va_arg(arguments, int); }
// @rewrite-fn-end
// @lowering-fn-end

int relay(int count, ...) {
  va_list arguments;
  va_start(arguments, count);
  int value = consume(arguments);
  va_end(arguments);
  return value;
}

int main(void) {
  printf("%d\n", relay(1, 37));
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn consume(mut {{arg[0-9]+}}: __SlateVaArgs) -> i32 {
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = unsafe { {{arg[0-9]+}}.next_arg::<i32>() };
// LOWERING-DAG:     return {{_v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn consume(mut {{arg[0-9]+}}: __SlateVaArgs) -> i32 {
// REWRITES-DAG:     return unsafe { {{arg[0-9]+}}.next_arg::<i32>() };
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
