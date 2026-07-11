#include <stdalign.h>
#include <stdio.h>
struct S {
  char a;
  double b;
};
int main(void) {
  printf("%d %d\n", (int)alignof(int), (int)alignof(double));
  printf("%d\n", (int)alignof(struct S));
  return 0;
}
