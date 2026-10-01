#include <stdio.h>

typedef struct {
  int  kind;
  int  size;
  _Bool is_unsigned;
} Type;

static _Bool from_int    = 5;
static _Bool from_zero   = 0;
static int   less        = 1 < 2;
static int   greater     = -1 > 2u;
static int   equal       = 3 == 3;
static int   both        = 1 && 0;
static int   either      = 0 || 7;
static int   float_order = 1.5 < 2.5;
static Type *uchar       = &(Type){1, 1, 1};
static Type  plain       = {2, 4, 0 != 0};

int main(void) {
  printf("%d %d %d %d %d %d %d %d\n", from_int, from_zero, less, greater, equal,
         both, either, float_order);
  printf("%d %d %d %d\n", uchar->kind, uchar->is_unsigned, plain.size,
         plain.is_unsigned);
  return 0;
}
