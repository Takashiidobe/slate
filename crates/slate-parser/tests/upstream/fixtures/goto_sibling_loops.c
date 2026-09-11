#include <stdio.h>

int main() {
  int i    = 0;
  int k    = 0;
  int sum  = 0;
  int prod = 1;
first:
  sum = sum + i;
  i   = i + 1;
  if (i < 5)
    goto first;
  k = 1;
second:
  prod = prod * k;
  k    = k + 1;
  if (k < 5)
    goto second;
  printf("%d %d\n", sum, prod);
  return 0;
}


