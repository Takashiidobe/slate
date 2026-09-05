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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn consume(mut {{arg[0-9]+}}: __SlateVaArgs) -> i32 {
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = unsafe { {{arg[0-9]+}}.next_arg::<i32>() };
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG:     let mut arguments: __SlateVaArgs = __SlateVaArgs::empty();
// LOWERING-AARCH64-GNU-DAG:     arguments = {{arg[0-9]+}}.clone();
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: i32 = unsafe { arguments.next_arg::<i32>() };
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn consume(mut {{arg[0-9]+}}: __SlateVaArgs) -> i32 {
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG:     unsafe { {{arg[0-9]+}}.next_arg::<i32>() }
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG:     let mut arguments: __SlateVaArgs = __SlateVaArgs::empty();
// REWRITES-AARCH64-GNU-DAG:     arguments = {{arg[0-9]+}}.clone();
// REWRITES-AARCH64-GNU-DAG:     unsafe { arguments.next_arg::<i32>() }
// SLATE-FILECHECK-END rewrites-aarch64-gnu
