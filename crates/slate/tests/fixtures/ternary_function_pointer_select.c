#include <stdio.h>

typedef int (*BinaryOp)(int, int);

static int add(int a, int b) { return a + b; }
static int sub(int a, int b) { return a - b; }

static int apply(int useAdd, int a, int b) {
  return (useAdd ? add : sub)(a, b);
}

int main(void) {
  int      useAdd = 1;
  BinaryOp op     = useAdd ? add : sub;
  printf("%d %d %d %d\n", op(10, 3), apply(0, 10, 3), apply(1, 4, 4),
         (useAdd ? sub : add)(9, 2));
  return 0;
}
