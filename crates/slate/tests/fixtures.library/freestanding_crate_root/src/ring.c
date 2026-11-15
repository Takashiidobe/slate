/* { dg-options "-ffreestanding" } */
#include "ring.h"

static int slots[4];
static int count;

int ring_push(int value) {
  if (count == 4) {
    panic("ring full: %d\n", value);
    return -1;
  }
  slots[count++] = value;
  return count;
}

int ring_sum(void) {
  int total = 0;
  for (int i = 0; i < count; i++)
    total += slots[i];
  return total;
}
