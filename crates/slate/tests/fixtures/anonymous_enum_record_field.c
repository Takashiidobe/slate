#include <stdio.h>

struct Conditional {
  enum { IN_THEN, IN_ELIF = 4, IN_ELSE } ctx;
};

int main(void) {
  struct Conditional conditional = {IN_THEN};
  conditional.ctx                = IN_ELSE;
  printf("%d %d\n", (int)conditional.ctx, IN_ELIF);
}
