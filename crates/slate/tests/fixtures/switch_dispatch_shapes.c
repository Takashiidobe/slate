#include <stdio.h>

enum Op { Add, Sub, Mul, Halt };

static int calls = 0;

static int next_value(int value) {
  calls++;
  return value;
}

static int stacked(int x) {
  switch (x) {
  case 1:
  case 2:
  case 3:
    return 10;
  default:
  case 9:
    return 90;
  case 4: {
    int doubled = x * 2;
    return doubled;
  }
  }
}

static int no_default(char c) {
  int out = 0;
  switch (c) {
  case 'a':
    out = 1;
    break;
  case 'z':
    out = 26;
    break;
  }
  return out;
}

static int nested(int outer, int inner) {
  int out = 0;
  switch (outer) {
  case 0:
    switch (inner) {
    case 0:
      out = 100;
      break;
    default:
      out = 101;
      break;
    }
    out += 1;
    break;
  default:
    out = -1;
    break;
  }
  return out;
}

static int run(const enum Op *ops) {
  int acc = 1;
  int i = 0;
  while (1) {
    enum Op op = ops[i];
    i++;
    switch (op) {
    case Add:
      acc += 3;
      break;
    case Sub:
      acc -= 1;
      continue;
    case Mul: {
      acc *= 2;
      break;
    }
    case Halt:
      return acc;
    }
    acc += 100;
  }
}

static int first_over(const int *values, int count, int limit) {
  int found = -1;
  for (int i = 0; i < count; i++) {
    if (values[i] > limit) {
      found = i;
      break;
    }
  }
  return found;
}

int main(void) {
  enum Op ops[] = {Add, Sub, Mul, Add, Halt};
  int values[] = {3, 8, 1, 12, 7};
  int picked = 0;
  switch (next_value(2)) {
  case 1:
    picked = 1;
    break;
  case 2:
    picked = 2;
    break;
  }
  printf("%d %d %d %d %d\n", stacked(2), stacked(9), stacked(4), stacked(42), stacked(0));
  printf("%d %d %d\n", no_default('a'), no_default('z'), no_default('q'));
  printf("%d %d %d\n", nested(0, 0), nested(0, 5), nested(1, 0));
  printf("%d %d %d\n", run(ops), first_over(values, 5, 10), first_over(values, 5, 50));
  printf("%d %d\n", picked, calls);
  return 0;
}
