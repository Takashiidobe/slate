#include <stdio.h>

int fallback(void);
int both_weak(void);
int lone_weak(void);
int defaults_used(void);
extern int threshold;
extern int shared_weak;
int *pointer_fallback(void);
int *lone_pointer_weak(void);

int main(void) {
  printf("%d %d %d %d %d %d %d %d\n", fallback(), both_weak(), lone_weak(),
         defaults_used(), threshold, shared_weak, *pointer_fallback(),
         *lone_pointer_weak());
  return 0;
}
