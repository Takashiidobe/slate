#include <stdio.h>
int native_value[2] = { 17, 29 };
int native_answer(void) { return 41; }
int asm_symbols(void);
int main(void) {
  printf("%d\n", asm_symbols());
  return 0;
}
