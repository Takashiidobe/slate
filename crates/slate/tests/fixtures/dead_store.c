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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = 9;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = noisy({{__v[0-9]+}});
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-NOT: let {{__v[0-9]+}}: i32 = 1;
// COMMON-REWRITES-NOT: let {{__v[0-9]+}}: i32 = 2;
// COMMON-REWRITES-NOT: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-REWRITES-DAG: noisy(9);
// SLATE-FILECHECK-END common-rewrites
