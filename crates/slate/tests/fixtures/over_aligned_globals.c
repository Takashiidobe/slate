#include <stdint.h>
#include <stdio.h>

struct Point {
  int x;
  int y;
};

_Alignas(64) static int counter = 3;
_Alignas(32) static uint32_t values[3] = {4, 5, 6};
static double weights[2] __attribute__((aligned(128))) = {0.5, 1.5};
_Alignas(16) static struct Point origin = {7, 8};
_Alignas(4096) static char page[16];
_Alignas(16) static char small;

static uint32_t *values_tail = &values[2];
static struct Point *origin_ptr = &origin;
static int *counter_ptr = &counter;

static int aligned(const void *pointer, uintptr_t alignment) {
  return (uintptr_t)pointer % alignment == 0;
}

static uint32_t total(const uint32_t *items, int count) {
  uint32_t sum = 0;
  for (int i = 0; i < count; i++) {
    sum += items[i];
  }
  return sum;
}

static int bump(void) {
  _Alignas(256) static int calls;
  calls += 1;
  return calls + aligned(&calls, 256);
}

int main(void) {
  counter += 10;
  values[1] = 50;
  *values_tail += 600;
  weights[0] *= 4.0;
  origin.y = 80;
  origin_ptr->x += 1;
  page[3] = 'p';
  small = 's';
  *counter_ptr *= 2;
  bump();
  printf("%d %d %d %d %d %d\n", aligned(&counter, 64), aligned(values, 32),
         aligned(weights, 128), aligned(&origin, 16), aligned(page, 4096),
         aligned(&small, 16));
  printf("%d %u %u %u %u\n", counter, values[0], values[1], values[2],
         total(values, 3));
  printf("%.2f %.2f %d %d %c %c %d\n", weights[0], weights[1], origin.x,
         origin.y, page[3], small, bump());
  printf("%zu %zu %zu\n", sizeof counter, sizeof values, sizeof origin);
  return counter % 9;
}
