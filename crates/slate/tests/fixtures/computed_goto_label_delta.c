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

int main(void) {
  const unsigned char code[] = {0, 1, 2, 1};
  // @lowering-begin
  // @rewrite-begin
  printf("%d\n", interpret(code, 4));
  // @rewrite-end
  // @lowering-end
  return 0;
}

// COMMON-DAG: {{__state[0-9]+}} = [{{[0-9]+}}, {{[0-9]+}}, {{[0-9]+}}][

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: *mut u8 = code.as_mut_ptr() as *mut u8;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = interpret({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: *mut u8 = code.as_mut_ptr() as *mut u8;
// COMMON-REWRITES-DAG: unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, interpret({{__v[0-9]+}}, 4)) };
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = c"%d\n".as_ptr() as *mut i8;
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = c"%d\n".as_ptr() as *mut u8;
// SLATE-FILECHECK-END rewrites-aarch64-gnu
