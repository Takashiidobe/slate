// REWRITES-DAG: #[linkage = "extern_weak"]
// REWRITES-DAG: static abs: Option<unsafe extern "C" fn(i32) -> i32>;
// REWRITES-NOT: fn weakref_alias
// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: weakref_target(35)
// REWRITES-DAG: abs.unwrap()(-53 as i32)
// REWRITES: {{^}}}

#include <stdio.h>
#include <stdlib.h>

int weakref_target(int value) { return value + 7; }

static int weakref_alias(int) __attribute__((weakref("weakref_target")));

static int weakref_external(int) __attribute__((weakref("abs")));

int main(void) {
  printf("%d %d %d\n", weakref_alias(35), weakref_target(41),
         weakref_external(-53));
  return 0;
}
