#include "shared.h"

#define PROJECT_WARNING_TOKEN expanded
#warning PROJECT_WARNING_TOKEN remains unexpanded

int square(int x) {
  struct shared_value value = {x, SHARED_READY};
  return value.value * value.value;
}
// LOWERING-DAG: slate_support::warning!("PROJECT_WARNING_TOKEN remains unexpanded");
// LOWERING-DAG: use crate::types::shared_mode_t;
// LOWERING-LABEL: {{^}}pub extern "C" fn square(
// LOWERING-DAG: value.mode = shared_mode_t::SHARED_READY;
// LOWERING: {{^}}}
