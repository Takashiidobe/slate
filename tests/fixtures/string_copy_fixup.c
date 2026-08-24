#include <stdio.h>
#include <string.h>

int main(void) {
  char copy[16]         = {0};
  char append[16]       = "foo";
  char trunc_copy[16]   = {0};
  char trunc_append[16] = "pre";

  strcpy(copy, "abc");
  strcat(append, "bar");
  strncpy(trunc_copy, "abcdef", 3);
  strncat(trunc_append, "suffix", 3);

  printf("%s %s %s %s %zu\n", copy, append, trunc_copy, trunc_append,
         strlen(trunc_append));
  return 0;
}
// REWRITES-DAG: let mut copy: String = "".to_owned();
// REWRITES-DAG: let mut append: String = "foo".to_owned();
// REWRITES-DAG: copy = "abc".to_owned();
// REWRITES-DAG: append.push_str("bar");
// REWRITES-DAG: trunc_copy = "abc".to_owned();
// REWRITES-DAG: trunc_append.push_str("suf");
// REWRITES-NOT: fn strcpy(
// REWRITES-NOT: fn strncpy(
// REWRITES-NOT: fn strcat(
// REWRITES-NOT: fn strncat(
