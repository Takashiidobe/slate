#include <stdio.h>

int main(void) {
  double d = 7.75;
  float f = (float)d;
  int i = (int)d;
  unsigned int u = (unsigned int)-1;
  long l = (long)i;
  unsigned char c = (unsigned char)260;
  _Bool b0 = (_Bool)0.0;
  _Bool b1 = (_Bool)-3;
  printf("%f\n", f);
  printf("%d\n", i);
  printf("%u\n", u);
  printf("%ld\n", l);
  printf("%u\n", c);
  printf("%d %d\n", b0, b1);
  return 0;
}
