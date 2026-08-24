#include <ctype.h>
#include <stdio.h>

static int next_lower(void) {
  static int c = 'a';
  return c++;
}

int main(void) {
  int upper = 'Q';
  int lower = 'q';
  int digit = '5';
  int punct = '!';
  int eof   = EOF;

  printf("%d %d %d %d %d\n", toupper(lower), toupper(digit), toupper(punct),
         toupper(upper), toupper(eof));
  printf("%d %d %d %d %d\n", tolower(upper), tolower(digit), tolower(punct),
         tolower(lower), tolower(eof));
  printf("%d %d\n", toupper(next_lower()), tolower(next_lower()));
  return 0;
}
// REWRITES-NOT: fn toupper(
// REWRITES-NOT: fn tolower(
// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: (lower as i32) >= 97 && (lower as i32) <= 122
// REWRITES-DAG: (upper as i32) >= 65 && (upper as i32) <= 90
// REWRITES-NOT: unsafe { toupper(
// REWRITES-NOT: unsafe { tolower(
// REWRITES: {{^}}}
