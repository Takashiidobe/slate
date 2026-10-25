/* { dg-options "-fplan9-extensions" } */
#include <stdio.h>

typedef struct base {
  int id;
} base_t;

struct derived {
  int extra;
  base_t;
};

static int read_id(base_t *b) { return b->id; }

int main(void) {
  struct derived d;
  d.extra = 7;
  d.id = 9;
  base_t *view = &d;
  printf("%d %d %d %d\n", read_id(&d), view->id, d.base_t.id, d.extra);
  return 0;
}
