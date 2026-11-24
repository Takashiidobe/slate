#include "../abi.h"
#include "dylink.h"
#include "dylink_path.h"
#include <stdio.h>

static int __attribute__((swiftcall))
check_context(int value, void *context __attribute__((swift_context)),
              uintptr_t ordinary) {
  return value + ((uintptr_t)context != ordinary_argument ? 23 : 0);
}

int main(void) {
  void *lib = dl_open(DEP_PATH);
  if (!lib) return 1;
  int (*invoke)(callback_fn *) = (int (*)(callback_fn *))dl_sym(lib, "call_cb");
  if (!invoke) return 2;
  printf("%d\n", invoke(check_context));
  dl_close(lib);
  return 0;
}
