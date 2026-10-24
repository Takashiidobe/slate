/* { dg-options "--flavor=gcc -fms-extensions" } */
#include <stdio.h>

struct point {
  int x;
  int y;
};

typedef struct point point_t;

struct labeled {
  struct point;
  int label;
};

struct typedef_member {
  point_t;
  int z;
};

int main(void) {
  struct labeled l = {{1, 2}, 3};
  struct typedef_member t = {{4, 5}, 6};
  printf("%d %d %d %d %d %d %zu\n", l.x, l.y, l.label, t.x, t.y, t.z, sizeof(struct labeled));
  return 0;
}
