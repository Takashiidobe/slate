#include <stdio.h>

static const char *describe(int code) {
  const char *Err;
  if (code == 0) {
    Err = "ok";
  } else {
    Err = "bad";
  }
  return Err;
}

static int pick(int Ok, int None) {
  int Some = Ok + None;
  return Some * 2;
}

int main(void) {
  printf("%s\n", describe(0));
  printf("%s\n", describe(1));
  printf("%d\n", pick(3, 4));
  return 0;
}
