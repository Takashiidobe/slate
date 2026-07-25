#include <math.h>
#include <stdio.h>
int main(void) {
  int e = 0;
  float ip = 0.0f;
  float x = 9.0f;
  float y = 2.0f;
  float m = frexpf(x, &e);
  float frac = modff(3.75f, &ip);
  printf("%.6f %.6f %.6f %.6f\n", sinf(0.5f), cosf(0.5f), sqrtf(x),
         fmodf(x, y));
  printf("%.6f %d %.6f %.6f\n", m, e, ip, frac);
  return 0;
}
