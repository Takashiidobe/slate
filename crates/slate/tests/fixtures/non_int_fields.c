#include <stdio.h>

struct Mixed {
  char tag;
  unsigned char code;
  float ratio;
  double total;
};

union Scalar {
  char tag;
  unsigned char code;
  float ratio;
  double total;
};

static int struct_values(void) {
  struct Mixed m;
  m.tag = 'A';
  m.code = 200;
  m.ratio = 1.5f;
  m.total = 2.25;
  printf("%c\n", m.tag);
  printf("%u\n", m.code);
  printf("%f\n", m.ratio);
  printf("%f\n", m.total);
  return m.code;
}

static double union_double(double value) {
  union Scalar s;
  s.total = value;
  return s.total;
}

static int union_char(char value) {
  union Scalar s;
  s.tag = value;
  return s.tag;
}

int main(void) {
  printf("%d\n", struct_values());
  printf("%f\n", union_double(4.5));
  printf("%d\n", union_char('Z'));
  return 0;
}
