#include <stdio.h>

int aliased(int);
int from_static(int);
int weak_alias(int);
int local_identity(void);

int main(void) {
  int (*through)(int) = aliased;
  printf("%d %d %d %d %d\n", aliased(83), from_static(5), weak_alias(1),
         through(2), local_identity());
  return 0;
}
