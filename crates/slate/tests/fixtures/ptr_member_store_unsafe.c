#include <stdio.h>

struct inner {
  int start;
  int end;
};

struct outer {
  struct inner buf;
  int error;
};

static void bump(int *p) { *p = *p + 10; }

static void init(struct outer *o) {
  o->buf.start = 1;
  o->buf.end = 2;
  o->error = 0;
  bump(&o->buf.start);
}

int main(void) {
  struct outer o;
  init(&o);
  printf("%d %d %d\n", o.buf.start, o.buf.end, o.error);
  return 0;
}
