#include "../src/shared.h"

extern int square(int x);
extern int bump(void);

int main(void) {
  struct shared_value value = {3, SHARED_READY};
  int result = square(value.value) + bump();
  return result == 10 && value.mode == SHARED_READY ? 0 : 1;
}
