#include <stdio.h>

int main(void) {
  void *first = __builtin_thread_pointer();
  void *second = __builtin_thread_pointer();
  unsigned char buffer[16] = {0};
  __builtin___clear_cache((char *)buffer, (char *)buffer + sizeof(buffer));
  printf("%d %d\n", first != 0, first == second);
  return 0;
}
