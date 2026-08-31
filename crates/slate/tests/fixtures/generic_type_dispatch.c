#include <stdio.h>

static int int_score(int value) { return value + 10; }
static int long_score(long value) { return (int)value + 20; }
static int pointer_score(const int *value) { return *value + 30; }

#define SCORE(value)                                                          \
  _Generic((value), int: int_score, long: long_score,                         \
           const int *: pointer_score)(value)

int main(void) {
  const int value = 7;
  int array[] = {5, 6};
  // @lowering-begin
  // @rewrite-begin
  int first = SCORE(value);
  int second = SCORE(8L);
  int third = SCORE((const int *)array);
  printf("%d %d %d\n", first, second, third);
  // @rewrite-end
  // @lowering-end
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = int_score({{_v[0-9]+}});
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = 8;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = long_score({{_v[0-9]+}});
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i32 = array.as_mut_ptr() as *mut i32;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = pointer_score({{_v[0-9]+}});
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = int_score({{_v[0-9]+}});
// REWRITES-DAG: let {{_v[0-9]+}}: i64 = 8;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = long_score({{_v[0-9]+}});
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i32 = array.as_mut_ptr() as *mut i32;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = pointer_score({{_v[0-9]+}});
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-DAG: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END rewrites
