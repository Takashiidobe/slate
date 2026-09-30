#include <stdio.h>

struct __slate_anonymous_struct_0 {
  int value;
};

int main(void) {
  struct __slate_anonymous_struct_0 named = {7};
  struct {
    int x;
    int y;
  } point = {3, 4};
  printf("%d\n", named.value + point.x + point.y);
  return 0;
}
