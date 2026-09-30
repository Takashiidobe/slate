#include <stdio.h>

static float half(float x) { return x * 0.5f; }
static double third(double x) { return x / 3.0; }

int main(void) {
  float  f0 = 0.1f;
  float  fmn = 1.17549435e-38f;
  float  fsub = 1.4e-45f;
  float  fmx = 3.40282347e+38f;
  double d0 = 0.1;
  double dsub = 4.9406564584124654e-324;
  double dmx = 1.7976931348623157e+308;
  double nz = -0.0;
  double neg = -2.5;
  double big = 1e300;
  double inf = 1e999;
  float  finf = -1e999f;
  printf("%a %a %a %a\n", f0, fmn, fsub, fmx);
  printf("%a %a %a %a %a %a\n", d0, dsub, dmx, nz, neg, big);
  printf("%f %f\n", inf, finf);
  printf("%a %a\n", half(3.0f), third(1.0));
  printf("%.17g %.9g\n", 1.0 / 3.0, 2.0f / 3.0f);
  return 0;
}
