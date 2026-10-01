#include <stdio.h>

struct Node {
  int value;
  struct Node *next;
};

static int calls;
static int slots[6];

static int idx(void) {
  calls += 1;
  return calls % 6;
}

static int *slot(void) {
  calls += 10;
  return &slots[2];
}

static int produce(void) {
  calls += 100;
  return 7;
}

static int twice(int value) { return value * 2; }

int main(void) {
  int a, b, c;
  a = b = c = produce();
  int d = (a += 3) * 2;
  int e = twice(b = 11);
  slots[idx()] += 5;
  slots[idx()] = slots[0] + 1;
  int f = (*slot() = 40) + 2;
  int g = (*slot() += 1);
  const char *text = "store";
  const char *cursor = text;
  char ch;
  int length = 0;
  while ((ch = *cursor++)) {
    length += ch == 'o';
  }
  int buffer[4] = {0};
  int *out = buffer;
  for (int i = 1; i <= 3; i++) {
    *out++ = i * i;
  }
  struct Node second = {2, 0};
  struct Node first = {1, &second};
  struct Node *node = &first;
  int total = 0;
  while ((node = node->next) != 0) {
    total += (node->value *= 5);
  }
  int flag = 0;
  int picked = (flag = a > 5) ? (b -= 1) : (c += 1);
  unsigned char narrow;
  int widened = (narrow = 300) + 1;
  printf("%d %d %d %d %d %d\n", a, b, c, d, e, calls);
  printf("%d %d %d %d %d %d %d\n", slots[0], slots[1], slots[2], slots[3], f, g, length);
  printf("%d %d %d %d %d %d\n", buffer[0], buffer[1], buffer[2], (int)(out - buffer), total, second.value);
  printf("%d %d %d %d\n", flag, picked, narrow, widened);
  return (a + b + c) & 0x7f;
}
