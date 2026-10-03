#include <stdint.h>
#include <stdio.h>

register uintptr_t stack_pointer __asm__("rsp");
register char *stack_pointer_bytes __asm__("rsp");

static int near(uintptr_t sp, const void *local) {
  uintptr_t here = (uintptr_t)local;
  uintptr_t distance = here > sp ? here - sp : sp - here;
  return sp != 0 && sp % 8 == 0 && distance < 65536;
}

static int read_integer(void) {
  int local = 1;
  return near(stack_pointer, &local);
}

static int read_pointer(void) {
  int local = 2;
  return near((uintptr_t)stack_pointer_bytes, &local);
}

int main(void) {
  printf("%d %d\n", read_integer(), read_pointer());
  return 0;
}
