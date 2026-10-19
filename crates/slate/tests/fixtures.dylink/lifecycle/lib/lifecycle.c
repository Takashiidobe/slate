#include "dylink.h"

static int loaded;
static int calls;

#ifdef _WIN32
__declspec(dllimport) int __stdcall DisableThreadLibraryCalls(void *);

int __stdcall DllMain(void *module, unsigned long reason, void *reserved) {
  (void)reserved;
  if (reason == 1) {
    DisableThreadLibraryCalls(module);
    loaded = 1;
  }
  return 1;
}
#else
__attribute__((constructor)) static void on_load(void) { loaded = 1; }
#endif

EXPORT int plug_loaded(void) { return loaded; }

EXPORT int plug_call(void) { return ++calls; }

EXPORT int *plug_calls_address(void) { return &calls; }
