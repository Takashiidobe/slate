#include <stdio.h>

struct Inner {
  int value;
  double weight;
};

struct Outer {
  int id;
  struct Inner inner;
  int items[4];
  struct Inner *link;
};

static struct Outer global_outer;

static void bump(int *slot, int by) { *slot += by; }

static void scale(double *slot, double factor) { *slot *= factor; }

static int *pick(struct Outer *outer, int which) {
  if (which == 0) {
    return &outer->id;
  }
  if (which == 1) {
    return &outer->inner.value;
  }
  return &outer->items[which - 2];
}

static struct Inner *inner_of(struct Outer *outer) { return &outer->inner; }

int main(void) {
  struct Outer local = {1, {2, 1.5}, {10, 20, 30, 40}, 0};
  struct Inner spare = {7, 0.25};
  local.link = &spare;

  bump(&local.id, 5);
  bump(&local.inner.value, 3);
  bump(&local.items[2], 100);
  scale(&local.inner.weight, 4.0);
  bump(&local.link->value, 9);
  scale(&local.link->weight, 8.0);

  int *slot = pick(&local, 3);
  *slot = *slot * 2;
  *pick(&local, 0) += 1000;

  struct Inner *inner = inner_of(&local);
  inner->value += 50;

  global_outer.id = 3;
  bump(&global_outer.id, 4);
  bump(&global_outer.inner.value, 11);
  scale(&global_outer.inner.weight, 2.0);
  int *global_slot = &global_outer.items[1];
  *global_slot = 77;
  global_outer.link = &global_outer.inner;
  bump(&global_outer.link->value, 1);

  printf("%d %d %d %d %d %d\n", local.id, local.inner.value, local.items[0],
         local.items[1], local.items[2], local.items[3]);
  printf("%.2f %d %.2f\n", local.inner.weight, spare.value, spare.weight);
  printf("%d %d %.2f %d\n", global_outer.id, global_outer.inner.value,
         global_outer.inner.weight, global_outer.items[1]);
  return local.id % 7;
}
