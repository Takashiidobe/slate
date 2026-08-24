#include "sanitized_input.h"
#include <limits.h>

#error blocked on \
purpose

#ifdef SANITIZED_LEFT
int selected(void) { return INT_MAX; }
#else
int selected(void) { return INCLUDED_VALUE; }
#endif

const char *source_name(void) { return __FILE__; }

int main(void) { return selected() == 0; }
// DIRECTIVES-DAG: #[cfg(feature = "sanitized_left")]
// DIRECTIVES-DAG: #[cfg(not(feature = "sanitized_left"))]
// DIRECTIVES-DAG: i32::MAX
// DIRECTIVES-DAG: tests/fixtures.cfg/sanitized_input.c
