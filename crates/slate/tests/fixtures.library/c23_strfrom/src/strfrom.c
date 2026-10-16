#include <math.h>
#include <stddef.h>
#include <stdlib.h>

struct measurement {
  long double value;
};

// @check-fn-begin
void set_measurement(struct measurement *measurement, long double value) {
  measurement->value = value + 1.0L;
}
// @check-fn-end

int format_pi(char *out, size_t n) { return strfroml(out, n, "%.1f", 3.5L); }

// @check-fn-begin
int root_to_int(void) { return sqrtl(4.0L) == 2.0L; }
// @check-fn-end
