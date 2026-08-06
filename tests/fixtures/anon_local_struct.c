#include <stdio.h>

typedef struct {
  int *start;
  int *end;
  int *pointer;
} buffer_t;

int main(void) {
  struct {
    int x;
    int y;
  } point = {3, 4};

  int      storage[4];
  buffer_t buf = {0, 0, 0};
  buf.start    = storage;
  buf.pointer  = storage;
  buf.end      = storage + 4;

  *buf.pointer = point.x + point.y;
  buf.pointer++;
  *buf.pointer = point.x * point.y;
  buf.pointer++;

  printf("%d %d\n", storage[0], storage[1]);
  printf("%ld\n", (long)(buf.pointer - buf.start));
  printf("%ld\n", (long)(buf.end - buf.start));
  return 0;
}
