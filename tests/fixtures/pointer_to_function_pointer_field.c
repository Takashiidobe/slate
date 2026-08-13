#include <stdio.h>
#include <stdlib.h>

typedef void (*test_fn)(void);

typedef struct {
  test_fn *tests;
  int      ntests;
} suite_t;

static int last_ran = -1;

static void test_a(void) { last_ran = 0; }

static void test_b(void) { last_ran = 1; }

int main(void) {
  suite_t suite;
  suite.tests    = malloc(2 * sizeof(test_fn));
  suite.tests[0] = test_a;
  suite.tests[1] = test_b;
  suite.ntests   = 2;

  for (int i = 0; i < suite.ntests; i++) {
    suite.tests[i]();
    printf("%d\n", last_ran);
  }

  free(suite.tests);
  return 0;
}
