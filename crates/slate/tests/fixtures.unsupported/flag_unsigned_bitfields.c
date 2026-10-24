/* { dg-options "--flavor=gcc -funsigned-bitfields" } */
int printf(const char *, ...);

typedef int plain_int;

struct bits {
  int a : 3;
  plain_int b : 3;
  signed int c : 3;
  char d : 3;
};

int main(void) {
  struct bits v;
  v.a = 7;
  v.b = 7;
  v.c = 7;
  v.d = 7;
  printf("%d %d %d %d\n", v.a, v.b, v.c, v.d);
  printf("%d\n", v.a - 8 < 0);
  return 0;
}
