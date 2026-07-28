#include <stdio.h>

static __complex__ int global_value = 17 + 19i;

int main(void) {
  __complex__ int first = 5 + 7i;
  __complex__ int second = -3 + 11i;
  __complex__ int imaginary = 13i;
  printf("%d %d %d %d %d %d %d %d\n", __real__ first, __imag__ first,
         __real__ second, __imag__ second, __real__ imaginary,
         __imag__ imaginary, __real__ global_value, __imag__ global_value);
  return 0;
}
