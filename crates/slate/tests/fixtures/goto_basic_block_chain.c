#include <stdio.h>

static int trace;
static int *saved;

static int mark(int digit) {
  trace = trace * 10 + digit;
  return digit;
}

static int run(int skip) {
  int result = mark(1);
  saved = &result;
  if (skip) goto entered;
  {
    int value = mark(2);
  entered:
    value = mark(3);
    result += value;
    *saved += mark(4);
  }
  goto alias;
  result = mark(9);
alias:
  goto finish;
finish:
  result += mark(5);
  printf("%d %d\n", result, trace);
  return result;
}

static int straight_line(void) {
  int result = mark(6);
  goto finish;
  result = mark(9);
finish:
  result += mark(7);
  result += mark(8);
  return result;
}

int main(void) {
  run(0);
  trace = 0;
  run(1);
  trace = 0;
  printf("%d\n", straight_line());
  printf("%d\n", trace);
  return 0;
}
