#include <stdio.h>

void keep(int (^callback)(int));
int fire(int x);

__attribute__((noinline)) static void register_scaler(int k) {
  keep(^(int x) { return k * x; });
}

__attribute__((noinline)) static void clobber_stack(void) {
  volatile unsigned char buffer[4096];
  for (unsigned i = 0; i < sizeof buffer; i++)
    buffer[i] = 0x41;
}

int main(void) {
  register_scaler(6);
  clobber_stack();
  printf("%d\n", fire(7));
  return 0;
}
