#include <stdio.h>

#define CHECK_VALUE(expression)                                                \
  ({                                                                           \
    __label__ failed, done;                                                    \
    int result;                                                                \
    if (!(expression))                                                         \
      goto failed;                                                             \
    result = 17;                                                               \
    goto done;                                                                 \
  failed:                                                                      \
    result = -5;                                                               \
  done:                                                                        \
    result;                                                                    \
  })

int main(void) {
  int value  = 0;
  int first  = CHECK_VALUE(++value == 1);
  int second = CHECK_VALUE(++value == 9);
  // @lowering-begin
  // @rewrite-begin
  printf("%d %d %d\n", first, second, value);
  // @rewrite-end
  // @lowering-end
  return 0;
}

// COMMON-DAG: let mut result: i32 = 0;
// COMMON-DAG: let mut result2: i32 = 0;

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = first;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = second;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = value;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 =
// LOWERING-DAG:     unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: unsafe { printf(c"%d %d %d\n".as_ptr(), first, second, value) };
// SLATE-FILECHECK-END rewrites
