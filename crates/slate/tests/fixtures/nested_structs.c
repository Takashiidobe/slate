#include <stdio.h>

struct Inner {
  int x;
  int y;
};

struct Outer {
  struct Inner a;
  int z;
};

int main(void) {
  struct Outer o;
  o.a.x = 3;
  o.a.y = 4;
  o.z = 5;
  printf("%d\n", o.a.x + o.a.y + o.z);

  struct Outer init = {{1, 2}, 3};
  printf("%d\n", init.a.x + init.a.y + init.z);
  return 0;
}
