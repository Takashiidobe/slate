// REWRITES-DAG: static cached_value: std::sync::OnceLock<i32> = std::sync::OnceLock::new();
// REWRITES-NOT: static mut cached_value
// REWRITES-NOT: computed
// REWRITES-LABEL: {{^}}fn get_value() -> i32 {
// REWRITES-DAG: *cached_value.get_or_init(|| compute())
// REWRITES: {{^}}}

#include <stdio.h>

static int cached_value = 0;
static int computed     = 0;

static int compute(void) { return 42; }

int get_value(void) {
  if (!computed) {
    cached_value = compute();
    computed     = 1;
  }
  return cached_value;
}

int main(void) {
  printf("%d\n", get_value());
  printf("%d\n", get_value());
  return 0;
}
