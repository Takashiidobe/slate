#include <stdio.h>

struct Pair {
  int left;
  int right;
};

struct Nested {
  struct Pair inner;
  int         tag;
};

struct WithArray {
  int data[3];
  int marker;
};

static int take_pair(struct Pair p) { return p.left * 10 + p.right; }

static struct Pair replace_left(struct Pair p, int v) {
  p.left = v;
  return p;
}

static int nested_total(struct Nested n) {
  return n.inner.left + n.inner.right + n.tag;
}

static int array_value(struct WithArray w) { return w.data[1] + w.marker; }

int main(void) {
  struct Pair      p = {2, 3};
  struct Pair      q = replace_left(p, 7);
  struct Nested    n = {{4, 5}, 6};
  struct WithArray w = {{8, 9, 10}, 11};
  printf("%d\n", take_pair(p));
  printf("%d\n", take_pair(q));
  printf("%d\n", nested_total(n));
  printf("%d\n", array_value(w));
  return 0;
}
// REWRITES-LABEL: {{^}}fn take_pair(
// REWRITES-DAG: p.left * 10 + p.right
// REWRITES: {{^}}}
// REWRITES-LABEL: {{^}}fn nested_total(
// REWRITES-DAG: n.inner.left + n.inner.right + n.tag
// REWRITES: {{^}}}
// REWRITES-LABEL: {{^}}fn array_value(
// REWRITES-DAG: w.data[1] + w.marker
// REWRITES: {{^}}}
