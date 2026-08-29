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
  return 0;
}
// REWRITES-DAG: union anon_0 {
// REWRITES-DAG: integer: i32,
// REWRITES-DAG: real: f32,
// REWRITES-DAG: struct container {
// REWRITES-DAG: __slate_anon_1: anon_0,
// REWRITES-DAG: value.__slate_anon_1.integer
