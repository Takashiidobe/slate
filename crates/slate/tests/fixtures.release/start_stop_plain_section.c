#include <stdio.h>
extern const int __start_slate_plain_set[];
extern const int __stop_slate_plain_set[];
__attribute__((section("slate_plain_set"))) const int plain_entry = 5;
int main(void) {
  int count = 0;
  for (const int *e = __start_slate_plain_set; e < __stop_slate_plain_set; e++) count += *e;
  printf("%d\n", count);
  return 0;
}
