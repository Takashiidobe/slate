#include "../abi.h"
#include "dylink.h"
#include "dylink_path.h"
#include <stdio.h>

static void __attribute__((preserve_most)) invoke_clobber(clobber_fn f) { f(); }

int main(void) {
  void *lib = dl_open(DEP_PATH);
  if (!lib) return 1;
  int (*invoke)(callback_fn *) = (int (*)(callback_fn *))dl_sym(lib, "call_cb");
  if (!invoke) return 2;
  printf("%d\n", invoke(invoke_clobber));
  dl_close(lib);
  return 0;
}
