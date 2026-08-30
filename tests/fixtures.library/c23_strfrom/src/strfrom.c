#include <stddef.h>
#include <stdlib.h>

struct measurement {
  long double value;
};

void set_measurement(struct measurement *measurement, long double value) {
  measurement->value = value;
}

int format_pi(char *out, size_t n) { return strfroml(out, n, "%.1f", 3.5L); }
// LOWERING-DAG: fn __slate_strfroml__ri32_pi8_usize_pi8_f80(
// LOWERING-LABEL: {{^}}pub extern "C" fn format_pi(
// LOWERING-DAG: unsafe { __slate_strfroml__ri32_pi8_usize_pi8_f80(
// LOWERING: {{^}}}
