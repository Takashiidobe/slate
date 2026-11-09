#include <stdio.h>
#include "dylink.h"
#include "dylink_path.h"
#define ODR_DEFINE_HERE
#include "odr.h"

typedef int (*add_fn)(int, int);

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
  int first = next_ticket();
  int second = other_unit();
  int third = next_ticket();
  printf("tickets %d %d %d\n", first, second, third);
  printf("twice %d\n", twice_counted(add(1, 2)));
  printf("thrice %d\n", thrice_counted(add(2, 3)));
  return 0;
}
