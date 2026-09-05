#include <stdio.h>

static int int_score(int value) { return value + 10; }
static int long_score(long value) { return (int)value + 20; }
static int pointer_score(const int *value) { return *value + 30; }

#define SCORE(value)                                                           \
  _Generic((value),                                                            \
      int: int_score,                                                          \
      long: long_score,                                                        \
      const int *: pointer_score)(value)

int main(void) {
  const int value   = 7;
  int       array[] = {5, 6};
  // @lowering-begin
  // @rewrite-begin
  int       first   = SCORE(value);
  int       second  = SCORE(8L);
  int       third   = SCORE((const int *)array);
  printf("%d %d %d\n", first, second, third);
  // @rewrite-end
  // @lowering-end
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = int_score({{__v[0-9]+}});
// LOWERING-DAG: let {{__v[0-9]+}}: i64 = 8;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = long_score({{__v[0-9]+}});
// LOWERING-DAG: let {{__v[0-9]+}}: *mut i32 = array.as_mut_ptr() as *mut i32;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = pointer_score({{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{__v[0-9]+}}: i32 = int_score(7 as i32);
// REWRITES-DAG: let {{__v[0-9]+}}: i32 = long_score(8 as i64);
// REWRITES-DAG: let {{__v[0-9]+}}: *mut i32 = array.as_mut_ptr() as *mut i32;
// REWRITES-DAG: unsafe { printf(c"%d %d %d\n".as_ptr(), {{__v[0-9]+}}, {{__v[0-9]+}}, pointer_score({{__v[0-9]+}})) };
// SLATE-FILECHECK-END rewrites
