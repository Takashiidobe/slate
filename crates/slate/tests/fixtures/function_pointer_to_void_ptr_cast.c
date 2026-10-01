#include <stdint.h>
#include <stdio.h>

typedef int (*Callback)(int);

static int add_one(int x) { return x + 1; }

static void *store_fn(void *fn) { return fn; }

int main(void) {
  void    *slot = (void *)add_one;
  Callback cb   = (Callback)slot;

  void    *slot2 = store_fn((void *)add_one);
  Callback cb2   = (Callback)slot2;

  printf("%d %d\n", cb(41), cb2(99));

  uintptr_t address = (uintptr_t)add_one;
  Callback  cb3     = (Callback)address;
  printf("%d %d\n", cb3(1), address == (uintptr_t)(void *)add_one);

  typedef long (*Other)(long, long);
  Other    other = (Other)add_one;
  Callback cb4   = (Callback)other;
  printf("%d\n", cb4(7));

  Callback none = (Callback)(void *)0;
  void    *gone = (void *)none;
  printf("%d %d\n", none == 0, gone == 0);
  return 0;
}
