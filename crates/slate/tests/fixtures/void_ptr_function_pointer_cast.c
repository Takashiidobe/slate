#include <stdio.h>

typedef int (*Callback)(int);

static int add_one(int x) { return x + 1; }

union FnBox {
  Callback fn;
  void    *ptr;
};

int main(void) {
  union FnBox box;
  box.fn        = add_one;
  void    *slot = box.ptr;
  Callback cb   = (Callback)slot;
  Callback none = (Callback)0;
  printf("%d %d\n", cb(41), none != NULL);
  return 0;
}
