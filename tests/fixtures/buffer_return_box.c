#include <stdio.h>
#include <stdlib.h>

static int *make(int n) {
  int *p = malloc(n * sizeof(int));
  for (int i = 0; i < n; i++) {
    p[i] = i * i;
  }
  return p;
}

static int *make8(void) {
  int *p = malloc(8 * sizeof(int));
  for (int i = 0; i < 8; i++) {
    p[i] = i;
  }
  return p;
}

static int *maybe(int n) {
  if (n < 0) {
    return NULL;
  }
  return malloc(n * sizeof(int));
}

static int *allocfree(int n) {
  int *p = malloc(n * sizeof(int));
  free(p);
  return malloc(n * sizeof(int));
}

int main(void) {
  // @rewrite-begin
  int *q = make(4);
  // @rewrite-end
  // @rewrite-begin
  int *r = make8();
  // @rewrite-end
  // @rewrite-begin
  int *m = maybe(2);
  // @rewrite-end
  // @rewrite-begin
  int *a = allocfree(3);
  // @rewrite-end
  printf("%d %d %d %d\n", q[3], r[7], (int)(m != NULL), (int)(a != NULL));
  free(q);
  free(r);
  if (m) {
    free(m);
  }
  free(a);
  return 0;
}
// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 4;
// REWRITES-DAG: q = Box::into_raw(make({{_v[0-9]+}})).cast::<i32>();
// REWRITES-DAG: r = Box::into_raw(make8()).cast::<i32>();
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 2;
// REWRITES-DAG: m = maybe({{_v[0-9]+}});
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = 3;
// REWRITES-DAG: a = allocfree({{_v[0-9]+}});
// SLATE-FILECHECK-END rewrites
