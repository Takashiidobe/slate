#include "shared.h"
#include <stdio.h>

int main(void) {
  struct Callback cb;
  cb.handler = add_ignore_b;
  printf("%d\n", call_handler(&cb, 5, 99));
  return 0;
}
