#include <stdio.h>

struct writer {
  const char *name;
  int phase;
};

struct context {
  struct writer super;
  int self;
  int crate;
  int Self;
};

static int super = 3;

static int combine(int self, int crate) {
  int Self = self * crate;
  return Self + super;
}

int main(void) {
  struct context ctx = {{"out", 2}, 5, 7, 11};
  ctx.super.phase += ctx.self;
  printf("%s %d %d\n", ctx.super.name, ctx.super.phase, ctx.crate + ctx.Self);
  printf("%d\n", combine(ctx.self, ctx.crate));
  return 0;
}
