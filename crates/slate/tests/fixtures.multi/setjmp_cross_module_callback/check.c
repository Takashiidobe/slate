#include <stdio.h>

void fail_now(void);

void check(int ok) {
  if (!ok) {
    fail_now();
  }
  printf("PASS\n");
}

void other_check(int ok) {
  (void)ok;
  printf("OTHER\n");
}
