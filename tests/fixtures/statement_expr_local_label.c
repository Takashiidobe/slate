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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = first;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = second;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = value;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 =
// COMMON-LOWERING-DAG:     unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: unsafe { printf(c"%d %d %d\n".as_ptr(), first, second, value) };
// SLATE-FILECHECK-END common-rewrites
