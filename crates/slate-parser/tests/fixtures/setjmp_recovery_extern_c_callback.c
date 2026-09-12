#include <setjmp.h>
#include <stdio.h>

static jmp_buf env;
static int     failures = 0;

static void fail_now(void) { longjmp(env, 1); }

static void check(int ok) {
  if (!ok) {
    fail_now();
  }
  printf("PASS\n");
}

static void run_case(void (*fn)(int), int ok) {
  if (setjmp(env)) {
    failures++;
    printf("FAIL\n");
    return;
  }
  fn(ok);
}

int main(void) {
  void (*fn)(int) = check;
  run_case(fn, 0);
  printf("failures: %d\n", failures);
  return 0;
}

// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]

// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
