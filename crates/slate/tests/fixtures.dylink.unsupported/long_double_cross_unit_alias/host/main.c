#include <stdio.h>
#include "dylink.h"
#include "dylink_path.h"

typedef long double (*step_fn)(long double);
typedef int (*check_fn)(step_fn);

extern long double alias_step(long double);
extern long double weak_step(long double);

int main(void) {
  void *lib = dl_open(DEP_PATH);
  if (!lib)
    return 1;
  check_fn check = (check_fn)dl_sym(lib, "check_step");
  if (!check)
    return 2;
  printf("strong=%d weak=%d\n", check(alias_step), check(weak_step));
  return dl_close(lib) ? 0 : 3;
}
