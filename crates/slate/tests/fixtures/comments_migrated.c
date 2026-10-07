#include <stdio.h>

int twice(int n) { return n * 2; }

static int folded(int n) {
  /* leading folded */
  int a = twice(n); /* trailing folded */
  /* leading folded return */
  return a + 1;
}

static int zeroed(int n) {
  int s;
  int j;
  s = 0; /* trailing zeroed init */
  /* leading zeroed loop */
  for (j = 0; j < n; j++) {
    s += j;
  }
  /* trailing zeroed return */
  return s;
}

static int dispatched(int n) {
  int acc = 0; /* trailing dispatched acc */
  int tmp;     /* trailing dispatched tmp */
top:
  /* leading dispatched add */
  acc += n;
  tmp = acc * 2; /* trailing dispatched assign */
  if (acc < 100) goto top;
  /* leading dispatched return */
  return tmp;
}

int main(void) {
  printf("%d %d %d\n", folded(3), zeroed(5), dispatched(7));
  return 0;
}
