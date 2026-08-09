#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
  intptr_t  signed_value   = -42;
  uintptr_t unsigned_value = 42;
  printf("%+6" PRIdPTR " %" PRIiPTR " %" PRIoPTR " %" PRIuPTR " %" PRIxPTR
         " %" PRIXPTR "\n",
         signed_value, signed_value, unsigned_value, unsigned_value,
         unsigned_value, unsigned_value);
  return 0;
}
