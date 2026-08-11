#include <setjmp.h>
#include <stdio.h>

jmp_buf g_env;
int failures = 0;

void fail_now(void) {
  longjmp(g_env, 1);
}

void run_case(void (*fn)(int), int ok) {
  if (setjmp(g_env)) {
    failures++;
    printf("FAIL\n");
    return;
  }
  fn(ok);
}
