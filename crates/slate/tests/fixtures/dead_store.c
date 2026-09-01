#include <stdio.h>

static int noisy(int x) {
  printf("noisy %d\n", x);
  return x;
}

int main(void) {
  // @lowering-begin
  // @rewrite-not-begin
  int dead = 1 + 2;
  // @rewrite-not-end
  // @lowering-end
  int kept = noisy(5);
  // @lowering-begin
  // @rewrite-begin
  noisy(9);
  // @rewrite-end
  // @lowering-end
  printf("%d\n", kept);
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 2;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 9;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = noisy({{_v[0-9]+}});
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-NOT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NOT: let {{_v[0-9]+}}: i32 = 2;
// REWRITES-NOT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-DAG: noisy(9);
// SLATE-FILECHECK-END rewrites
