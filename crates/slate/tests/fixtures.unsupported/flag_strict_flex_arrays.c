/* { dg-options "-fstrict-flex-arrays=3 -O2" } */
#include <stdio.h>
#include <stdlib.h>

struct one_trailing {
  int count;
  int items[1];
};

struct flexible {
  int count;
  int items[];
};

static size_t one_size(struct one_trailing *p) { return __builtin_dynamic_object_size(p->items, 1); }

static size_t flex_size(struct flexible *p) { return __builtin_dynamic_object_size(p->items, 1); }

int main(void) {
  struct one_trailing *one = malloc(sizeof *one + 9 * sizeof(int));
  struct flexible *flex = malloc(sizeof *flex + 10 * sizeof(int));
  printf("%zu %zu\n", one_size(one), flex_size(flex));
  free(one);
  free(flex);
  return 0;
}
