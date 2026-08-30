#include <stdio.h>

int main(void) {
  const volatile int qualified = 9;
  typeof_unqual(qualified) copy = qualified;
  typeof(qualified) preserved = 12;
  int *pointer = nullptr;
  constexpr int width = 7;
  unsigned _BitInt(width) narrow = 100;
  int unqualified = __builtin_types_compatible_p(typeof(copy), int);
  int still_qualified =
      __builtin_types_compatible_p(typeof(preserved), const volatile int);
  printf("%d %d %d %d %d\n", copy, preserved, pointer == nullptr,
         (int)narrow, unqualified + still_qualified);
  return 0;
}
