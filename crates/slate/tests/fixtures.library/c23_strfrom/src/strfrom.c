#include <math.h>
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

// @lowering-fn-begin
// @rewrite-fn-begin
int root_to_int(void) { return sqrtl(4.0L) == 2.0L; }
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-DAG: let {{__v[0-9]+}}: LongDouble = {{arg[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG: unsafe {
// LOWERING-DAG:     (*{{arg[0-9]+}}).value = {{__v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: #[unsafe(no_mangle)]
// LOWERING-DAG: pub extern "C-unwind" fn root_to_int() -> i32 {
// LOWERING-DAG:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]);
// LOWERING-DAG:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_sqrtl__rf80_f80({{__v[0-9]+}}) };
// LOWERING-DAG:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-DAG: let {{__v[0-9]+}}: LongDouble = {{arg[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     (*{{arg[0-9]+}}).value = {{__v[0-9]+}};
// REWRITES-DAG: }
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub extern "C-unwind" fn root_to_int() -> i32 {
// REWRITES-DAG:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]);
// REWRITES-DAG:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_sqrtl__rf80_f80({{__v[0-9]+}}) };
// REWRITES-DAG:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-DAG:     {{__v[0-9]+}}
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
