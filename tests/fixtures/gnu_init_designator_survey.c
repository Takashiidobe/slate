#include <stdio.h>

union FlexUnion {
  int  value;
  char data[];
};

struct OnlyFlex {
  char data[];
};

union Castable {
  int   i;
  float f;
};

enum Forward;
enum Forward { FORWARD_A, FORWARD_B };

struct Point {
  int x;
  int y;
};

struct Sized {
  int n;
  int data[];
};

static struct Sized sized = {3, {10, 20, 30}};

struct NestedOuter {
  struct Sized inner;
};

int main(void) {
  int range_values[10] = {[2 ... 5] = 9};
  int old_index[3] = {[1] 11};
  struct Point p = {x: 1, y: 2};
  int            five = 5;
  union Castable c    = (union Castable)five;
  enum Forward   f = FORWARD_B;

  printf("%d %d %d %d\n", range_values[3], old_index[1], p.x + p.y, c.i);
  printf("%d\n", (int)f);
  printf("%zu %zu\n", sizeof(union FlexUnion), sizeof(struct OnlyFlex));
  printf("%d %d %d\n", sized.data[0], sized.data[1], sized.data[2]);
  printf("%zu\n", sizeof(struct NestedOuter));
  return 0;
}
