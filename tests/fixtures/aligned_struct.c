#include <stddef.h>
#include <stdio.h>

struct __attribute__((aligned(16))) Aligned {
  char a;
  int b;
};

static int next_value(void) { return 9; }

static int effectful_case(void) {
  struct Aligned effectful;
  effectful.a = next_value();
  effectful.b = 7;
  return effectful.a + effectful.b;
}

static int repeated_case(void) {
  struct Aligned repeated;
  repeated.a = 1;
  repeated.a = 2;
  repeated.b = 3;
  return repeated.a + repeated.b;
}

static int dependent_case(void) {
  struct Aligned dependent;
  dependent.b = 8;
  dependent.a = dependent.b;
  return dependent.a + dependent.b;
}

static int counter;

static void touch(void) { counter++; }

static int interrupted_case(void) {
  struct Aligned interrupted;
  interrupted.a = 4;
  touch();
  interrupted.b = 6;
  return interrupted.a + interrupted.b + counter;
}

int main(void) {
  struct Aligned s;
  s.a = 5;
  s.b = 0x1234;

  printf("%zu %zu\n", sizeof(struct Aligned), _Alignof(struct Aligned));
  printf("%zu %zu\n", offsetof(struct Aligned, a), offsetof(struct Aligned, b));
  printf("%d %x\n", s.a, s.b);
  printf("%d %d %d %d\n", effectful_case(), repeated_case(), dependent_case(),
         interrupted_case());
  return 0;
}
