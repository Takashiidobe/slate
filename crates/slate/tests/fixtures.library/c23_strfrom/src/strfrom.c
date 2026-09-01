#include <stddef.h>
#include <stdlib.h>

struct measurement {
  long double value;
};

void set_measurement(struct measurement *measurement, long double value) {
  // @lowering-begin
  // @rewrite-begin
  measurement->value = value + 1.0L;
  // @rewrite-end
  // @lowering-end
}

int format_pi(char *out, size_t n) { return strfroml(out, n, "%.1f", 3.5L); }

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-DAG: let {{_v[0-9]+}}: LongDouble = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG: unsafe {
// LOWERING-DAG: (*{{arg[0-9]+}}).value = {{_v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: unsafe {
// REWRITES-DAG: (*{{arg[0-9]+}}).value = {{arg[0-9]+}} + LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
