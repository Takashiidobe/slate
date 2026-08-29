// REWRITES-DAG: #[linkage = "extern_weak"]
// REWRITES-DAG: static abs: Option<unsafe extern "C" fn(i32) -> i32>;
// REWRITES-NOT: fn weakref_alias

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
