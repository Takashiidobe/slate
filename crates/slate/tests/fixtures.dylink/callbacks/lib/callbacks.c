#include "dylink.h"

typedef int (*visit_fn)(int value, void *context);
typedef void (*log_fn)(const char *message, int code);

static log_fn current_logger;

EXPORT int plug_map(const int *values, int n, visit_fn visit, void *context) {
  int total = 0;
  for (int i = 0; i < n; i++) total += visit(values[i], context);
  return total;
}

EXPORT void plug_set_logger(log_fn logger) { current_logger = logger; }

EXPORT void plug_emit(int code) {
  if (current_logger) current_logger("from library", code);
}

EXPORT int plug_apply_twice(int (*f)(int), int v) { return f(f(v)); }

EXPORT int (*plug_pick(int which))(int, int);

static int plug_add(int a, int b) { return a + b; }
static int plug_sub(int a, int b) { return a - b; }

int (*plug_pick(int which))(int, int) { return which ? plug_sub : plug_add; }
