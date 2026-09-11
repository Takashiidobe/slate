#include <stdio.h>

int main() {
  volatile int pick_a = 1;
  volatile int pick_b = 0;
  int          x      = 0;
  int          y      = 0;

  if (pick_a)
    goto a2;
a1:
  x = x + 1;
  if (x < 3)
    goto a2;
  goto second;
a2:
  x = x + 2;
  if (x < 4)
    goto a1;

second:
  if (pick_b)
    goto b2;
b1:
  y = y + 3;
  if (y < 9)
    goto b2;
  goto done;
b2:
  y = y + 5;
  if (y < 11)
    goto b1;

done:
  printf("%d %d\n", x, y);
  return 0;
}


