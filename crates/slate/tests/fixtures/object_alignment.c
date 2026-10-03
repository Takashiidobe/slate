#include <stdint.h>
#include <stdio.h>

typedef unsigned long long v2uq __attribute__((vector_size(16)));

static char pad0;
static uint64_t table[64][64];
static char pad1;
static uint64_t small[2] = {3, 4};
static char pad2;
static _Alignas(32) uint32_t aligned32[3];
static char pad3;
static uint8_t bytes[16];

static int aligned(const void *pointer, uintptr_t alignment) {
  return ((uintptr_t)pointer & (alignment - 1)) == 0;
}

static uint64_t sum_vectors(const uint64_t *values, int count) {
  v2uq total = {0, 0};
  for (int i = 0; i < count; i += 2) {
    total += *(const v2uq *)(values + i);
  }
  return total[0] + total[1];
}

static int locals(int depth) {
  char tag = (char)depth;
  uint64_t row[4] = {1, 2, 3, (uint64_t)depth};
  char tag2 = tag;
  _Alignas(64) unsigned char line[8] = {0};
  short marker = 7;
  uint8_t block[32];
  __attribute__((aligned(16))) uint32_t word = 5;
  for (int i = 0; i < 32; i++) {
    block[i] = (uint8_t)(i + depth);
  }
  int ok = aligned(row, 16) && aligned(line, 64) && aligned(block, 16) && aligned(&word, 16);
  ok = ok && sum_vectors(row, 4) == 6u + (uint64_t)depth;
  if (depth > 0) {
    ok = ok && locals(depth - 1);
  }
  return ok + tag2 - tag + marker - 7 + line[0] + block[0] - depth + (int)word - 5;
}

static int dispatched(int n) {
  int count = 0;
  uint64_t values[2] = {10, 20};
again:
  if (!aligned(values, 16)) {
    return -1;
  }
  values[0] += sum_vectors(values, 2);
  if (++count < n) {
    goto again;
  }
  return (int)(values[0] % 1000);
}

int main(void) {
  for (int i = 0; i < 64; i++) {
    for (int j = 0; j < 64; j++) {
      table[i][j] = (uint64_t)(i * 64 + j);
    }
  }
  printf("table %d\n", aligned(table, 16));
  printf("small %d\n", aligned(small, 16));
  printf("aligned32 %d\n", aligned(aligned32, 32));
  printf("bytes %d\n", aligned(bytes, 16));
  printf("table sum %llu\n", (unsigned long long)sum_vectors(table[5], 64));
  printf("small sum %llu\n", (unsigned long long)sum_vectors(small, 2));
  printf("locals %d\n", locals(5));
  printf("dispatched %d\n", dispatched(4));
  return pad0 + pad1 + pad2 + pad3;
}
