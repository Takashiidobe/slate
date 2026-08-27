#include <stdint.h>

static int frame_address_is_plausible(void) {
  void *fp = __builtin_frame_address(0);
  return (uintptr_t)fp > 0x1000;
}

int main(void) { return frame_address_is_plausible() ? 0 : 1; }
