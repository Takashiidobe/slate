#include <ctype.h>
#include <stdio.h>

int classify(char c) { return isalpha(c); }

int main(void) {
  if (classify('A')) {
    printf("yes\n");
  } else {
    printf("no\n");
  }
  return 0;
}
// REWRITES-LABEL: {{^}}fn classify(
// REWRITES-DAG: isalpha(
// REWRITES-NOT: is_ascii_alphabetic
// REWRITES: {{^}}}
