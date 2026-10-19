#include <stdio.h>
#include "dylink.h"
#include "dylink_path.h"

struct tally {
  int calls;
  int weight;
};

static int weigh(int value, void *context) {
  struct tally *tally = (struct tally *)context;
  tally->calls++;
  return value * tally->weight;
}

static void log_message(const char *message, int code) { printf("log: %s (%d)\n", message, code); }

static int triple(int v) { return v * 3; }

int main(void) {
  void *lib = dl_open(DEP_PATH);
  if (!lib) {
    printf("open failed\n");
    return 1;
  }
  int (*map)(const int *, int, int (*)(int, void *), void *) =
      (int (*)(const int *, int, int (*)(int, void *), void *))dl_sym(lib, "plug_map");
  void (*set_logger)(void (*)(const char *, int)) =
      (void (*)(void (*)(const char *, int)))dl_sym(lib, "plug_set_logger");
  void (*emit)(int) = (void (*)(int))dl_sym(lib, "plug_emit");
  int (*apply_twice)(int (*)(int), int) =
      (int (*)(int (*)(int), int))dl_sym(lib, "plug_apply_twice");
  int (*(*pick)(int))(int, int) = (int (*(*)(int))(int, int))dl_sym(lib, "plug_pick");
  if (!map || !set_logger || !emit || !apply_twice || !pick) {
    printf("resolve failed\n");
    return 2;
  }

  int values[4] = {1, 2, 3, 4};
  struct tally tally = {0, 7};
  printf("map = %d\n", map(values, 4, weigh, &tally));
  printf("calls = %d\n", tally.calls);

  emit(1);
  set_logger(log_message);
  emit(2);
  emit(3);
  set_logger(0);
  emit(4);

  printf("apply_twice = %d\n", apply_twice(triple, 5));
  printf("pick(0)(10, 3) = %d\n", pick(0)(10, 3));
  printf("pick(1)(10, 3) = %d\n", pick(1)(10, 3));

  dl_close(lib);
  return 0;
}
