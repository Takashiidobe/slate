#include <stdio.h>

char *mutable_pick(int i) {
  if (i == 0) {
    return "mut";
  }
  return "other";
}

const char *const_pick(int i) {
  if (i == 0) {
    return "const";
  }
  return "other";
}

unsigned char *bytes_pick(void) { return (unsigned char *)"bytes"; }

int main(void) {
  printf("%s %s %s\n", mutable_pick(0), const_pick(0), (char *)bytes_pick());
  return 0;
}


