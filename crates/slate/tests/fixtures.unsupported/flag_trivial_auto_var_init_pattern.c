/* { dg-options "-ftrivial-auto-var-init=pattern" } */
int printf(const char *, ...);

struct record {
  int count;
  char name[4];
};

static void show(void) {
  int scalar;
  struct record r;
  unsigned char *bytes = (unsigned char *)&r;
  printf("%x %02x %02x\n", (unsigned)scalar, bytes[0], bytes[7]);
}

int main(void) {
  show();
  return 0;
}
