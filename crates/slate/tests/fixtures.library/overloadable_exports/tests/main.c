#include <stdio.h>
int __attribute__((overloadable)) scale(int x);
int __attribute__((overloadable)) scale(long x);
int renamed_entry(void);
int main(void) {
  printf("%d %d %d\n", scale(2), scale(2L), renamed_entry());
  return 0;
}
