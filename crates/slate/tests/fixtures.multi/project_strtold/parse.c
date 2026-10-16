#include <stdlib.h>

// @check-fn-begin
int parse_value(void) {
  char       *end;
  long double value = strtold("2.75", &end);
  return *end == '\0' ? (int)value : -1;
}
// @check-fn-end
