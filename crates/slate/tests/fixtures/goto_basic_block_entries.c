#include <stdio.h>

static volatile int observed;

static int forwarding_cycle(int stop) {
  if (stop) return 7;
first:
  goto second;
second:
  goto first;
}

static int run(int mode) {
  int sum = 0;
  int visits = 0;
  int checks = 0;
  int steps = 0;
  int i = 0;
  if (mode) goto second;
first:
  sum += 2;
  observed = sum;
  ++visits;
  if (visits < 4) goto alias;
  goto join;
alias:
  goto second;
second:
  sum += 3;
  observed = sum;
  ++visits;
  if (visits < 4) goto first;
join:
  for (; ++checks && i < 4; ++steps, ++i) {
    if (i == 1) continue;
    switch (i) {
    case 0:
      sum += 4;
      goto middle;
    case 2:
      sum += 5;
    middle:
      sum += observed;
      break;
    default:
      goto done;
    }
  }
done:
  printf("%d %d %d %d %d\n", sum, visits, checks, steps, observed);
  return sum;
}

int main(void) {
  run(0);
  run(1);
  printf("%d\n", forwarding_cycle(1));
  return 0;
}
