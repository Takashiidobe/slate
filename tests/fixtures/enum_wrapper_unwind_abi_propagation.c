#include <setjmp.h>
#include <stdio.h>

enum Status { STATUS_OK, STATUS_FAIL };
typedef enum Status Processor(int x);
typedef void Callback(int x);

struct Dispatcher {
  Processor *run;
};

static jmp_buf env;
static int failures = 0;
static Callback *g_callback;

static enum Status risky(int x) {
  if (x == 3) {
    longjmp(env, 1);
  }
  return STATUS_OK;
}

static void panicky_callback(int x) {
  if (x == 2) {
    longjmp(env, 1);
  }
  printf("callback %d\n", x);
}

static enum Status content_like(int x) {
  g_callback(x);
  return STATUS_OK;
}

int main(void) {
  struct Dispatcher d;
  g_callback = panicky_callback;

  d.run = risky;
  for (int i = 0; i < 5; i++) {
    if (setjmp(env)) {
      failures++;
      printf("recovered risky %d\n", i);
      continue;
    }
    d.run(i);
  }

  d.run = content_like;
  for (int i = 0; i < 5; i++) {
    if (setjmp(env)) {
      failures++;
      printf("recovered content_like %d\n", i);
      continue;
    }
    d.run(i);
  }

  printf("failures=%d\n", failures);
  return 0;
}
