#include <stdio.h>

#define VC __attribute__((vectorcall))

struct d2 {
  double a, b;
};

static int VC wide_then_int(long long a, int b) { return (int)a + b; }

static double VC hva(struct d2 r, int k) { return r.a * k + r.b; }

static struct d2 VC hva_result(double a, double b) {
  struct d2 r = {a, b};
  return r;
}

int main(void) {
  struct d2 r = {1.5, 2.25};
  printf("%d\n", wide_then_int(40, 2));
  printf("%.2f\n", hva(r, 3));
  printf("%.2f\n", hva_result(4.0, 0.5).b);
  return 0;
}
