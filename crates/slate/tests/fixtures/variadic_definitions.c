#include <stdarg.h>
#include <stdio.h>

static int ignore_rest(int first, ...) { return first * 3; }

static double promoted(int count, ...) {
  va_list ap;
  va_start(ap, count);
  double total = 0;
  for (int i = 0; i < count; i++) {
    total += va_arg(ap, double);
  }
  int letter = va_arg(ap, int);
  int small = va_arg(ap, int);
  va_end(ap);
  return total + letter + small;
}

static const char *pick(int index, ...) {
  va_list ap;
  va_start(ap, index);
  const char *chosen = 0;
  for (int i = 0; i <= index; i++) {
    chosen = va_arg(ap, const char *);
  }
  va_end(ap);
  return chosen;
}

static int next_pair(va_list *ap) {
  int left = va_arg(*ap, int);
  int right = va_arg(*ap, int);
  return left * right;
}

static int dot(int pairs, ...) {
  va_list ap;
  va_start(ap, pairs);
  int total = 0;
  for (int i = 0; i < pairs; i++) {
    total += next_pair(&ap);
  }
  va_end(ap);
  return total;
}

static long sum_longs(int count, ...) {
  va_list ap;
  va_start(ap, count);
  long total = 0;
  while (count-- > 0) {
    total += va_arg(ap, long);
  }
  va_end(ap);
  return total;
}

static long chain(int seed, ...) {
  va_list ap;
  va_start(ap, seed);
  long extra = va_arg(ap, long);
  unsigned long mask = va_arg(ap, unsigned long);
  va_end(ap);
  return sum_longs(3, (long)seed, extra, (long)(mask & 0xff)) +
         ignore_rest(seed, 1, 2.0, "three");
}

static void *same_pointer(void *first, ...) {
  va_list ap;
  va_start(ap, first);
  void *second = va_arg(ap, void *);
  va_end(ap);
  return second == first ? first : second;
}

int main(void) {
  char c = 'A';
  short s = -12;
  float f = 1.25f;
  int value = 9;
  printf("%d\n", ignore_rest(14));
  printf("%.3f\n", promoted(2, f, 2.5, c, s));
  printf("%s %s\n", pick(0, "zero", "one", "two"), pick(2, "zero", "one", "two"));
  printf("%d\n", dot(3, 1, 2, 3, 4, 5, 6));
  printf("%ld\n", chain(5, 1000000000000L, 0x1234UL));
  printf("%d\n", *(int *)same_pointer(&value, &value));
  return dot(1, 6, 7) % 40;
}
