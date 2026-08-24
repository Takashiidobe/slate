#include <stdlib.h>

int parse_value(void) {
  char       *end;
  long double value = strtold("2.75", &end);
  return *end == '\0' ? (int)value : -1;
}
// LOWERING-NOT: #[cfg(target_arch =
