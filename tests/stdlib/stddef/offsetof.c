#include <stddef.h>
#include <stdio.h>
struct S {
  char   a;
  int    b;
  double c;
};
int main(void) {
  printf("%d %d\n", (int)offsetof(struct S, b), (int)offsetof(struct S, c));
  return 0;
}
