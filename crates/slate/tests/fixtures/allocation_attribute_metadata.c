#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

__attribute__((malloc, alloc_size(1), alloc_align(2))) static void *
allocate(size_t size, size_t alignment) {
  (void)alignment;
  return malloc(size);
}

__attribute__((malloc, alloc_size(1, 2))) static void *
allocate_array(size_t count, size_t size) {
  return calloc(count, size);
}

__attribute__((assume_aligned(16, 4))) static void *
offset_aligned(void *pointer) {
  return (char *)pointer + 4;
}

int main(void) {
  int *values = allocate(3 * sizeof(int), _Alignof(int));
  int *zeroed = allocate_array(2, sizeof(int));
  values[0] = 7;
  values[1] = 9;
  int *second = offset_aligned(values);
  printf("%d %d %d\n", values[0], *second, zeroed[1]);
  free(zeroed);
  free(values);
  return 0;
}
