#include <stdio.h>
#include "dylink.h"
#include "dylink_path.h"

typedef int (*add_fn)(int, int);

int format_sum(char *out, size_t size, int sum);

int main(void) {
  void *lib = dl_open(DEP_PATH);
  if (!lib) {
    printf("open failed\n");
    return 1;
  }
  add_fn add = (add_fn)dl_sym(lib, "plug_add");
  if (!add) {
    printf("resolve failed\n");
    return 2;
  }
  char text[32];
  int written = format_sum(text, sizeof text, add(3, 4));
  printf("%s (%d)\n", text, written);
  return 0;
}
