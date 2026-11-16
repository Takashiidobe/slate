#include <stdio.h>

extern int alpha_alias;
int alpha_read(void);
int zeta_sum(void);

__attribute__((section("slate_cgu_data"))) int main_value = 4;

int main(void) {
  alpha_alias += main_value;
  printf("%d %d %d\n", zeta_sum(), alpha_read(), alpha_alias);
  return 0;
}
