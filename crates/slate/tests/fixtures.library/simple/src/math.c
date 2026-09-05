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
// REWRITES-DAG: pub extern "C-unwind" fn square({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG:     let mut value: shared_value = shared_value {
// REWRITES-DAG:         value: 0,
// REWRITES-DAG:         mode: shared_mode_t::SHARED_IDLE,
// REWRITES-DAG:     };
// REWRITES-DAG:     value.value = {{arg[0-9]+}};
// REWRITES-DAG:     value.mode = shared_mode_t::SHARED_READY;
// REWRITES-DAG:     value.value * value.value
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG: #[unsafe(no_mangle)]
// REWRITES-X86_64-GNU-DAG: pub extern "C-unwind" fn square({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-X86_64-GNU-DAG:     let mut value: shared_value = shared_value {
// REWRITES-X86_64-GNU-DAG:         value: 0,
// REWRITES-X86_64-GNU-DAG:         mode: shared_mode_t::SHARED_IDLE,
// REWRITES-X86_64-GNU-DAG:     };
// REWRITES-X86_64-GNU-DAG:     value.value = {{arg[0-9]+}};
// REWRITES-X86_64-GNU-DAG:     value.mode = shared_mode_t::SHARED_READY;
// REWRITES-X86_64-GNU-DAG:     value.value * value.value
// REWRITES-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG: #[unsafe(no_mangle)]
// REWRITES-AARCH64-GNU-DAG: pub extern "C-unwind" fn square({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-AARCH64-GNU-DAG:     let mut value: shared_value = shared_value {
// REWRITES-AARCH64-GNU-DAG:         value: 0,
// REWRITES-AARCH64-GNU-DAG:         mode: shared_mode_t::SHARED_IDLE,
// REWRITES-AARCH64-GNU-DAG:     };
// REWRITES-AARCH64-GNU-DAG:     value.value = {{arg[0-9]+}};
// REWRITES-AARCH64-GNU-DAG:     value.mode = shared_mode_t::SHARED_READY;
// REWRITES-AARCH64-GNU-DAG:     value.value * value.value
// REWRITES-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-aarch64-gnu
