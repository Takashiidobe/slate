/* { dg-options "-mms-bitfields" } */
int printf(const char *, ...);

struct mixed_units {
  char a : 3;
  int b : 4;
  char c;
};

struct widening {
  int x : 3;
  long long y : 3;
  int z : 3;
};

int main(void) {
  struct mixed_units m = {1, 2, 3};
  struct widening w = {1, 2, 3};
  printf("%zu %zu\n", sizeof(struct mixed_units), sizeof(struct widening));
  printf("%d %d %d %d %d %d\n", m.a, m.b, m.c, w.x, (int)w.y, w.z);
  return 0;
}
