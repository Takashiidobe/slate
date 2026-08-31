#include <stdio.h>

static int add(int a, int unused) { return a + 1; }

static int get_used(int a, int b) { return a + b; }

static int remove_two(int a, int unused_a, int unused_b) { return a + 2; }

static int address_taken(int a, int unused) { return a + 4; }

int main(void) {
  // @lowering-begin
  // @rewrite-begin
  int (*indirect)(int, int) = address_taken;
  printf("%d %d %d %d\n", add(5, 10), get_used(1, 2), remove_two(3, 4, 5),
         indirect(8, 9));
  // @rewrite-end
  // @lowering-end
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 5;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 10;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = add({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 2;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = get_used({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 3;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 4;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 5;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = remove_two({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 8;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 9;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { Some(address_taken).unwrap()({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 5;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 10;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = add({{_v[0-9]+}}, {{_v[0-9]+}});
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 2;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = get_used({{_v[0-9]+}}, {{_v[0-9]+}});
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 3;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 4;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 5;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = remove_two({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}});
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 8;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 9;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = unsafe { Some(address_taken).unwrap()({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-DAG: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END rewrites
