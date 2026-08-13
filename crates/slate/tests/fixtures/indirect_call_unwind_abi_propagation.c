#include <setjmp.h>
#include <stdio.h>

typedef void Callback(int x);
typedef void Dispatch(int x, int y);

struct Ctx {
  Dispatch *run;
};

static jmp_buf   env;
static int       failures = 0;
static Callback *g_callback;

static void quiet_callback(int x) { printf("quiet %d\n", x); }

static void panicky_callback(int x) {
  if (x == 2) {
    longjmp(env, 1);
  }
  printf("panicky %d\n", x);
}

static void dispatcher(int x, int y) {
  (void)y;
  g_callback(x);
}

int main(void) {
  struct Ctx c;
  c.run = dispatcher;

  g_callback = quiet_callback;
  for (int i = 0; i < 2; i++) {
    if (setjmp(env)) {
      failures++;
      printf("recovered quiet %d\n", i);
      continue;
    }
    c.run(i, 0);
  }

  g_callback = panicky_callback;
  for (int i = 0; i < 5; i++) {
    if (setjmp(env)) {
      failures++;
      printf("recovered panicky %d\n", i);
      continue;
    }
    c.run(i, 0);
  }

  printf("failures=%d\n", failures);
  return 0;
}
