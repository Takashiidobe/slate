#include <stdio.h>

int __attribute__((visibility("default"))) default_fn(void) { return 1; }

int __attribute__((visibility("hidden"))) hidden_fn(void) { return 2; }

int __attribute__((visibility("protected"))) protected_fn(void) { return 3; }

int __attribute__((visibility("default")))   default_global   = 4;
int __attribute__((visibility("hidden")))    hidden_global    = 5;
int __attribute__((visibility("protected"))) protected_global = 6;

int main(void) {
  printf("%d %d %d %d %d %d\n", default_fn(), hidden_fn(), protected_fn(),
         default_global, hidden_global, protected_global);
  return 0;
}
// LOWERING-DAG: pub static mut default_global
// LOWERING-DAG: pub extern "C" fn default_fn
// LOWERING-DAG: pub static mut protected_global
// LOWERING-DAG: pub extern "C" fn protected_fn
// LOWERING-DAG: {{^}}static mut hidden_global
// LOWERING-NOT: pub static mut hidden_global
// LOWERING-NOT: pub extern "C" fn hidden_fn
// LOWERING-LABEL: {{^}}fn hidden_fn(
// LOWERING: {{^}}}
