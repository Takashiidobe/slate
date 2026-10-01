#include <limits.h>
#include <stdio.h>

struct Counter {
  int hits;
  double total;
};

static int global_count = 10;
static int calls;

static int *next_slot(int *slots) {
  calls++;
  return slots + calls;
}

int main(void) {
  unsigned u = UINT_MAX;
  unsigned before = u++;
  unsigned after  = u;
  unsigned back   = --u;
  printf("%u %u %u\n", before, after, back);

  signed char sc = 127;
  sc++;
  printf("%d\n", sc);
  unsigned short us = 0;
  int old_us = us--;
  printf("%d %u\n", old_us, (unsigned)us);

  long long ll = LLONG_MIN + 1;
  long long old_ll = ll--;
  printf("%lld %lld\n", old_ll, ll);

  double d = 1.5;
  double d1 = d++;
  double d2 = ++d;
  double d3 = d--;
  printf("%g %g %g %g\n", d1, d2, d3, d);
  float f = -0.5f;
  float f1 = ++f;
  float f2 = f++;
  printf("%g %g %g\n", f1, f2, f);

  int values[5] = {0, 10, 20, 30, 40};
  int *p        = values;
  int *old      = p++;
  int at_p = *p;
  int at_next = *++p;
  printf("%d %d %d\n", *old, at_p, at_next);
  int was = (int)(p-- - values);
  printf("%d %d\n", was, (int)(p - values));
  printf("%d\n", (*p)++);
  printf("%d\n", *p--);
  printf("%d\n", values[1]);

  int i = 1;
  int r = values[i++]++;
  printf("%d %d %d\n", r, i, values[1]);
  r = ++values[i--];
  printf("%d %d %d\n", r, i, values[2]);

  int slots[4] = {0, 0, 0, 0};
  (*next_slot(slots))++;
  ++*next_slot(slots);
  printf("%d %d %d %d calls=%d\n", slots[0], slots[1], slots[2], slots[3], calls);

  struct Counter counter = {0, 0.0};
  struct Counter *cp     = &counter;
  counter.hits++;
  ++cp->hits;
  cp->total++;
  int hits = cp->hits++;
  printf("%d %d %g\n", hits, counter.hits, counter.total);

  int g1 = global_count--;
  int g2 = --global_count;
  printf("%d %d\n", g1, g2);

  _Bool flag = 0;
  flag++;
  _Bool still = flag++;
  printf("%d %d\n", flag, still);
  flag--;
  printf("%d\n", flag);
  flag--;
  printf("%d\n", flag);

  int n = 3, total = 0;
  while (n--)
    total += n;
  printf("%d %d\n", n, total);
  return 0;
}
