#include <stddef.h>
#include <stdlib.h>

struct FlexibleArray {
  size_t count;
  int values[];
};

int main(void) {
  if (sizeof(struct FlexibleArray) != sizeof(size_t)) {
    return 1;
  }

  struct FlexibleArray *flexible =
      malloc(sizeof(*flexible) + 3 * sizeof(flexible->values[0]));
  if (flexible == NULL) {
    return 2;
  }

  flexible->count = 3;
  for (size_t index = 0; index < flexible->count; ++index) {
    flexible->values[index] = (int)index + 1;
  }

  int total = 0;
  for (size_t index = 0; index < flexible->count; ++index) {
    total += flexible->values[index];
  }

  free(flexible);
  return total == 6 ? 0 : 3;
}
