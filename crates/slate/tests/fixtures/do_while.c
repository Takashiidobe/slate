#include <stdio.h>

static int sum_do_while(int n) {
  int total = 0;
  int i     = 1;
  do {
    total += i;
    i++;
  } while (i <= n);
  return total;
}

static int runs_once_when_false(void) {
  int total = 0;
  do {
    total += 7;
  } while (0);
  return total;
}

static int continue_checks_condition(int n) {
  int total = 0;
  int i     = 0;
  do {
    i++;
    if (i % 2 == 0) {
      continue;
    }
    total += i;
  } while (i < n);
  return total;
}

int main(void) {
  printf("%d\n", sum_do_while(5));
  printf("%d\n", sum_do_while(0));
  printf("%d\n", runs_once_when_false());
  printf("%d\n", continue_checks_condition(6));
  return 0;
}
// REWRITES-LABEL: {{^}}fn sum_do_while(
// REWRITES-DAG: loop {
// REWRITES-DAG: total += i;
// REWRITES-DAG: i += 1;
// REWRITES-NOT: {{^        \{$}}
// REWRITES: {{^}}}
// REWRITES-LABEL: {{^}}fn continue_checks_condition(
// REWRITES-DAG: loop {
// REWRITES-DAG: i += 1;
// REWRITES-DAG: if i % 2 != 0 {
// REWRITES-NOT: {{^            \{$}}
// REWRITES: {{^}}}
