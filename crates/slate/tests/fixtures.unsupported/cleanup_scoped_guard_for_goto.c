#include <stdio.h>

struct guard {
  int *lock;
};

static void guard_release(struct guard *g) {
  if (g->lock)
    (*g->lock)--;
}

static struct guard guard_take(int *lock, int ok) {
  struct guard g = {ok ? lock : 0};
  if (g.lock)
    (*g.lock)++;
  return g;
}

#define scoped_guard_label(lock, ok, label)                                    \
  for (struct guard scope __attribute__((cleanup(guard_release))) =            \
           guard_take(lock, ok);                                               \
       scope.lock || !1; ({ goto label; }))                                    \
    if (0) {                                                                   \
    label:                                                                     \
      break;                                                                   \
    } else

static int set_value(int *lock, int *slot, int value, int ok) {
  int ret = -22;
  scoped_guard_label(lock, ok, done_set) {
    *slot = value;
    ret = 0;
  }
  return ret;
}

static int read_value(int *lock, int *slot) {
  scoped_guard_label(lock, 1, done_read) { return *slot + *lock * 100; }
  return -1;
}

int main(void) {
  int lock = 0, slot = 0;
  int a = set_value(&lock, &slot, 7, 1);
  printf("%d %d %d\n", a, lock, slot);
  int b = set_value(&lock, &slot, 9, 0);
  printf("%d %d %d\n", b, lock, slot);
  int c = read_value(&lock, &slot);
  printf("%d %d\n", c, lock);
  return 0;
}
