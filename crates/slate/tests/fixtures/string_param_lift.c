#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_num(char *s) { return atoi(s); }

int forward_num(char *s) { return parse_num(s); }

int text_len(char *s) { return (int)strlen(s); }

int main(void) {
  char digits[] = "42";
  char word[]   = "hello";
  printf("%d %d\n", forward_num(digits), text_len(word));
  return 0;
}
// REWRITES-DAG: fn parse_num(s: &str) -> i32
// REWRITES-DAG: fn forward_num(s: &str) -> i32
// REWRITES-DAG: fn text_len(s: &str) -> i32
// REWRITES-DAG: __slate_runtime::parse_i32(s)
// REWRITES-DAG: parse_num(s)
// REWRITES-DAG: s.len()
// REWRITES-DAG: forward_num(digits)
// REWRITES-DAG: text_len(word)
// REWRITES-NOT: fn atoi(
// REWRITES-NOT: fn strlen(
