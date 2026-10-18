#include <stdatomic.h>
#include <stdio.h>

struct Small {
  char a[3];
};

struct Large {
  int values[5];
};

struct Holder {
  char                 head;
  _Atomic struct Small value;
  char                 tail;
};

_Atomic struct Small small;
_Atomic struct Large large;
static struct Holder holder;

int main(void) {
  printf("%zu %zu\n", sizeof(small), _Alignof(_Atomic struct Small));
  printf("%zu %zu\n", sizeof(large), _Alignof(_Atomic struct Large));
  printf("%zu\n", sizeof(holder));

  struct Small loaded = small;
  printf("%d %d %d\n", loaded.a[0], loaded.a[1], loaded.a[2]);
  small = (struct Small){{7, 8, 9}};
  loaded = atomic_load(&small);
  printf("%d %d %d\n", loaded.a[0], loaded.a[1], loaded.a[2]);

  atomic_store_explicit(&large, ((struct Large){{10, 20, 30, 40, 50}}), memory_order_release);
  struct Large big = atomic_load_explicit(&large, memory_order_acquire);
  printf("%d %d\n", big.values[0], big.values[4]);
  big.values[4] = 99;
  large = big;
  struct Large again = large;
  printf("%d\n", again.values[4]);

  holder.head = 'h';
  holder.value = (struct Small){{4, 5, 6}};
  holder.tail = 't';
  struct Small inner = holder.value;
  printf("%c %d %d %d %c\n", holder.head, inner.a[0], inner.a[1], inner.a[2], holder.tail);
  return 0;
}
