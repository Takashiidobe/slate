#include <stdio.h>

extern int *const __start_slate_symbol_refs[];
extern int *const __stop_slate_symbol_refs[];
extern int shared_beta;

__attribute__((section("slate_shared_data"))) int shared_alpha = 3;

__asm__(".pushsection slate_symbol_refs,\"aw\"\n"
        ".balign 8\n"
        ".quad shared_alpha\n"
        ".quad shared_beta\n"
        ".popsection");

int main(void) {
  int sum = 0;
  for (int *const *ref = __start_slate_symbol_refs; ref < __stop_slate_symbol_refs; ref++)
    sum = sum * 10 + **ref;
  printf("%d %d\n", sum, shared_beta);
  return 0;
}
