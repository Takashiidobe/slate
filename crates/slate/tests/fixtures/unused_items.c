#include <stdio.h>

struct totally_unused {
  int a;
  int b;
};

struct linked_a {
  int              x;
  struct linked_b *link;
};

struct linked_b {
  int              y;
  struct linked_a *link;
};

enum truly_dead { DEAD_A, DEAD_B };

enum color { RED, GREEN, BLUE };

static int hidden_static = 5;
static int live_static   = 7;

int compute(void) {
  int x = hidden_static;
  return 42;
}

int main(void) {
  enum color c = GREEN;
  printf("%d\n", c);
  printf("%d\n", live_static);
  printf("%d\n", compute());
  return 0;
}
// REWRITES-DAG: enum color
// REWRITES-DAG: mut c: color = color::RED
// REWRITES-DAG: c = color::GREEN
// REWRITES-DAG: c as u32
// REWRITES-DAG: live_static
// REWRITES-NOT: totally_unused
// REWRITES-NOT: linked_a
// REWRITES-NOT: linked_b
// REWRITES-NOT: truly_dead
// REWRITES-NOT: hidden_static
