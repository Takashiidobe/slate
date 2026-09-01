#include "shared.h"

#define PROJECT_WARNING_TOKEN expanded
#warning PROJECT_WARNING_TOKEN remains unexpanded

// @rewrite-fn-begin
int square(int x) {
  struct shared_value value = {x, SHARED_READY};
  return value.value * value.value;
}
// @rewrite-fn-end

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: #[unsafe(no_mangle)]
// REWRITES-DAG: pub extern "C" fn square({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG: let mut value: shared_value = shared_value {
// REWRITES-DAG: value: 0,
// REWRITES-DAG: mode: shared_mode_t::SHARED_IDLE,
// REWRITES-DAG: };
// REWRITES-DAG: value.value = {{arg[0-9]+}};
// REWRITES-DAG: value.mode = shared_mode_t::SHARED_READY;
// REWRITES-DAG: return value.value * value.value;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
