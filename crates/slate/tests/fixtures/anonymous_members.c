#include <stdio.h>

struct container {
  int prefix;
  union {
    int   integer;
    float real;
  };
  struct {
    int x;
    int y;
  };
};

int main(void) {
  struct container value = {0};
  value.prefix           = 3;
  value.integer          = 31;
  value.x                = 37;
  value.y                = 41;
  printf("%d %d %d %d %zu\n", value.prefix, value.integer, value.x, value.y,
         sizeof(value));
  value.real = 2.5f;
  printf("%d\n", (int)value.real);
  struct container positional = {1, {2}, {3, 4}};
  struct container designated = {.y = 7, .integer = 6, .prefix = 5};
  printf("%d %d %d %d\n", positional.prefix, positional.integer, positional.x,
         positional.y);
  printf("%d %d %d %d\n", designated.prefix, designated.integer, designated.x,
         designated.y);
  return 0;
}
