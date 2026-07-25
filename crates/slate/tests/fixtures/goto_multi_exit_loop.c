#include <stdio.h>

int main() {
  int i = 0;
  int sum = 0;
loop:
  sum = sum + i;
  if (sum > 100)
    goto overflow;
  i = i + 1;
  if (i < 5)
    goto loop;
  printf("%d\n", sum);
  return 0;
overflow:
  printf("overflow\n");
  return 0;
}
