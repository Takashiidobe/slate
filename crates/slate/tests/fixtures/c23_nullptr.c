#include <stddef.h>
#include <stdio.h>

int main(void) {
  volatile nullptr_t null_value = nullptr;
  int *null_pointer = nullptr;

  printf("%d %d\n", null_value == nullptr, null_pointer == nullptr);
  return 0;
}
