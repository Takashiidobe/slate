#include <stdio.h>

struct Point {
  int x;
  int y;
};

static int global_base = 100;

static int counter(void) {
  static int count;
  return ++count;
}

static int other_counter(void) {
  static int count = 50;
  return count++;
}

static int *history(int value) {
  static int slots[4];
  static int next;
  slots[next++ % 4] = value;
  return slots;
}

static struct Point *moved(int dx) {
  static struct Point point = {1, 2};
  point.x += dx;
  point.y *= 2;
  return &point;
}

static int *base_pointer(void) {
  static int *pointer = &global_base;
  return pointer;
}

static int depth(int n) {
  static int deepest;
  if (n > deepest)
    deepest = n;
  return n == 0 ? deepest : depth(n - 1);
}

static double running_mean(double sample) {
  static double sum;
  static int samples;
  sum += sample;
  return sum / ++samples;
}

static const char *name_of(int index) {
  static const char *const names[] = {"zero", "one", "two"};
  return names[index];
}

static int loop_scoped(void) {
  int total = 0;
  for (int i = 0; i < 3; i++) {
    static int seen = 7;
    total += seen++;
  }
  return total;
}

static int tally(void) {
  static int counter = 100;
  return counter++;
}

int main(void) {
  int c1 = counter();
  int c2 = counter();
  int o1 = other_counter();
  int o2 = other_counter();
  int c3 = counter();
  printf("%d %d %d %d %d\n", c1, c2, o1, o2, c3);
  int t1 = tally();
  int t2 = tally();
  printf("%d %d %d\n", t1, t2, counter());

  for (int i = 1; i <= 6; i++)
    history(i * 10);
  int *slots = history(70);
  printf("%d %d %d %d\n", slots[0], slots[1], slots[2], slots[3]);

  moved(3);
  struct Point *point = moved(4);
  printf("%d %d\n", point->x, point->y);
  point->x = 0;
  printf("%d\n", moved(1)->x);

  *base_pointer() += 5;
  int through = *base_pointer();
  printf("%d %d\n", global_base, through);

  int deep1 = depth(4);
  int deep2 = depth(2);
  printf("%d %d\n", deep1, deep2);
  running_mean(1.0);
  running_mean(2.0);
  printf("%g\n", running_mean(6.0));
  printf("%s %s\n", name_of(2), name_of(0));
  int first  = loop_scoped();
  int second = loop_scoped();
  printf("%d %d\n", first, second);
  return 0;
}
