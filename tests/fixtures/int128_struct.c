#include <stdio.h>

struct Wide {
  int tag;
  __int128 value;
  unsigned __int128 uvalue;
};

int main(void) {
  struct Wide w;
  w.tag = 7;
  w.value = -1234567890123456789;
  w.uvalue = 12345678901234567890ULL;
  w.value += 1;
  w.uvalue *= 2;

  printf("%d\n", w.tag);
  printf("%llu\n", (unsigned long long)(w.value >> 64));
  printf("%llu\n", (unsigned long long)w.value);
  printf("%llu\n", (unsigned long long)(w.uvalue >> 64));
  printf("%llu\n", (unsigned long long)w.uvalue);
  printf("%zu\n", sizeof(struct Wide));
  return 0;
}
