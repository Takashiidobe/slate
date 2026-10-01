#include <stdint.h>
#include <stdio.h>

enum Color { RED, GREEN = 5, BLUE };
enum Signed { NEG = -3, POS = 3 };
enum Big { BIG = 0xffffffffu };

static int as_int(enum Color color) { return color; }
static enum Color from_int(int value) { return value; }
static enum Signed flip(enum Signed value) { return (enum Signed)-value; }

int main(void) {
  printf("%d %d %d\n", as_int(RED), as_int(GREEN), as_int(BLUE));
  enum Color color = from_int(6);
  printf("%d %d\n", color == BLUE, color + 1);
  color = (enum Color)42;
  printf("%d\n", (int)color);
  color++;
  printf("%d\n", color);
  printf("%d %d\n", flip(NEG), flip(POS) < 0);
  printf("%u %d\n", (unsigned)BIG, BIG > 0);
  long long wide = NEG;
  unsigned char narrow = (unsigned char)NEG;
  printf("%lld %u\n", wide, (unsigned)narrow);

  int values[4] = {10, 20, 30, 40};
  int *p        = values;
  uintptr_t address = (uintptr_t)p;
  int *back         = (int *)address;
  printf("%d %d\n", back == p, *back);
  printf("%d\n", (int)((uintptr_t)(p + 3) - address));
  intptr_t signed_address = (intptr_t)(p + 2);
  printf("%d\n", *(int *)signed_address);

  void *opaque           = &values[1];
  const int *read_only   = opaque;
  int *writable          = (int *)read_only;
  *writable             += 5;
  printf("%d\n", values[1]);

  unsigned char *bytes = (unsigned char *)&values[0];
  printf("%u %u\n", (unsigned)bytes[0], (unsigned)bytes[1]);

  void *null = (void *)0;
  printf("%d %d %d\n", null == 0, !null, (intptr_t)null == 0);
  int *from_zero = (int *)(uintptr_t)0;
  printf("%d\n", from_zero == NULL);
  printf("%d\n", (_Bool)p);
  return 0;
}
