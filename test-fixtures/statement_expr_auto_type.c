#include <stdio.h>

#define EXCHANGE(pointer, replacement)                                        \
  ({                                                                          \
    __auto_type exchange_pointer = (pointer);                                 \
    __auto_type exchange_value = *exchange_pointer;                           \
    *exchange_pointer = (replacement);                                        \
    exchange_value;                                                           \
  })

int main(void) {
  long values[] = {4, 9, 16};
  int index = 0;
  long old = EXCHANGE(&values[index++], 25L);
  printf("%ld %ld %d\n", old, values[0], index);
  return 0;
}
