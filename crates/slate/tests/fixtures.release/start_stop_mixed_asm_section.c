#include <stdio.h>

extern const int __start_slate_mixed_set[];
extern const int __stop_slate_mixed_set[];

__attribute__((used, section("slate_mixed_set"))) static const int from_variable = 7;

__asm__(".pushsection slate_mixed_set,\"a\"\n"
        ".balign 4\n"
        ".long 11\n"
        ".popsection");

int main(void) {
  int sum = 0;
  int count = 0;
  for (const int *entry = __start_slate_mixed_set; entry < __stop_slate_mixed_set; entry++) {
    sum += *entry;
    count++;
  }
  printf("%d %d\n", count, sum);
  return 0;
}
