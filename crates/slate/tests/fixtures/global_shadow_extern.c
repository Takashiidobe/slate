#include <stdio.h>

extern int late;
int total = 10;
static long scale;

struct Tally {
  int  hits;
  long weight;
};

struct Tally tally = {1};

static void bump(int *slot, int by) { *slot += by; }

static int shadow_param(int total) { return total * 2; }

static void record(struct Tally *t, long weight) {
  t->hits += 1;
  t->weight += weight;
}

int main(void) {
  int before = total;
  {
    int total = 3;
    bump(&total, 4);
    printf("local %d\n", total);
  }
  bump(&total, 5);
  scale = 7;
  record(&tally, scale);
  record(&tally, scale * 2);
  printf("%d %d %d\n", before, total, shadow_param(total));
  printf("%d %ld\n", tally.hits, tally.weight);
  printf("late %d\n", late);
  return 0;
}

int late = 99;
