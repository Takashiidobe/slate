#include <stdio.h>

union Value {
  int i;
  float f;
};

int main(void) {
  union Value v = {258};
  printf("%d\n", v.i);
  v.f = 1.5f;
  printf("%.1f\n", v.f);
  return 0;
}
