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
// REWRITES-DAG: let mut value: shared_value = shared_value { value: 0, mode: shared_mode_t::SHARED_IDLE };
// REWRITES-DAG: value.value = {{arg[0-9]+}};
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = shared_mode_t::SHARED_READY as u32;
// REWRITES-DAG: value.mode = shared_mode_t::SHARED_READY;
// REWRITES-DAG: return value.value * value.value;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites

// LOWERING-DAG: slate_support::warning!("PROJECT_WARNING_TOKEN remains unexpanded");
// LOWERING-DAG: use crate::types::shared_mode_t;
// LOWERING-LABEL: {{^}}pub extern "C" fn square(
// LOWERING-DAG: value.mode = shared_mode_t::SHARED_READY;
// LOWERING: {{^}}}
