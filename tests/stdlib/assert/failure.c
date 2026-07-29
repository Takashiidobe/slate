#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static void handle_abort(int signal_number) {
  _Exit(signal_number == SIGABRT ? 0 : 1);
}

int main(void) {
  if (freopen("assert_failure.stderr", "w", stderr) == NULL)
    return 2;
  if (signal(SIGABRT, handle_abort) == SIG_ERR)
    return 3;

  puts("before");
  fflush(stdout);
  assert(0);
  return 4;
}
