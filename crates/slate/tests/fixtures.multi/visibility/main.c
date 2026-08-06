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
