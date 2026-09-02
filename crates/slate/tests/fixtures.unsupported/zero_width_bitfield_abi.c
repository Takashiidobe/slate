#include <stdio.h>

typedef struct {
  int value;
  unsigned : 0;
} __attribute__((aligned(16))) AlignedTail;

typedef struct {
  char tag;
  int : 0;
  int value;
} MidBarrier;

typedef struct __attribute__((packed)) {
  char tag;
  int : 0;
  int value;
} PackedBarrier;

__attribute__((noinline)) static int take_aligned(AlignedTail value) {
  return value.value + (int)sizeof(value);
}

__attribute__((noinline)) static MidBarrier round_trip(MidBarrier value) {
  value.value += value.tag;
  return value;
}

__attribute__((noinline)) static int take_packed(PackedBarrier value) {
  return value.tag + value.value + (int)sizeof(value);
}

int main(void) {
  AlignedTail   aligned = {.value = 7};
  MidBarrier    middle  = {.tag = 3, .value = 20};
  PackedBarrier packed  = {.tag = 4, .value = 30};
  middle                = round_trip(middle);
  printf("%d %d %d %zu %zu %zu\n", take_aligned(aligned), middle.value,
         take_packed(packed), sizeof(aligned), sizeof(middle), sizeof(packed));
  return 0;
}
