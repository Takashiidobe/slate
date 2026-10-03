#include <stdio.h>
#include <stdlib.h>
unsigned int _mm_getcsr(void);
void _mm_pause(void);
int main(void) {
  _mm_pause();
  unsigned int csr = _mm_getcsr();
  printf("%d\n", (csr & 0x1f80) == 0x1f80);
  return abs(0);
}
