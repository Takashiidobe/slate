#include "dylink.h"

EXPORT int check_step(long double (*step)(long double)) {
  return step(1.0L) == 0x1.0000000000000002p0L;
}
