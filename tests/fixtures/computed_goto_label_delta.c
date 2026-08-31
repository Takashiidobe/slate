#include <stdio.h>

static int interpret(const unsigned char *code, int length) {
  static const int offsets[] = {&&add - &&dispatch, &&double_it - &&dispatch,
                                &&subtract - &&dispatch};
  int index = 0;
  int value = 1;

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

int main(void) {
  const unsigned char code[] = {0, 1, 2, 1};
  // @lowering-begin
  // @rewrite-begin
  printf("%d\n", interpret(code, 4));
  // @rewrite-end
  // @lowering-end
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: *mut u8 = code.as_mut_ptr() as *mut u8;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 4;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = interpret({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: *mut u8 = code.as_mut_ptr() as *mut u8;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 4;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = interpret({{_v[0-9]+}}, {{_v[0-9]+}});
// REWRITES-DAG: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END rewrites

// COMMON-DAG: {{__state[0-9]+}} = [{{[0-9]+}}, {{[0-9]+}}, {{[0-9]+}}][
