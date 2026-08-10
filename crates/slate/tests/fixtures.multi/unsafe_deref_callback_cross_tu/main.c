#include "shared.h"
#include <stdio.h>

int main(void) {
  int value = 41;
  struct Callback cb;
  cb.handler = deref_and_add;
  int direct = deref_and_add(&value);
  int via_cb = call_handler(&cb, &value);
  printf("%d %d\n", direct, via_cb);
  return 0;
}
