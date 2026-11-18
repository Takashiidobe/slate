/* { dg-options "-freg-struct-return" } */
#include <stdio.h>

struct split {
  char a[3];
  char b;
};

static struct split make_split(int v) {
  struct split s = {{(char)v, (char)(v + 1), (char)(v + 2)}, (char)(v + 3)};
  return s;
}

int main(void) {
  struct split s = make_split(4);
  printf("%d %d %d %d\n", s.a[0], s.a[1], s.a[2], s.b);
  return 0;
}
