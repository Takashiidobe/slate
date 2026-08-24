#include <stdio.h>

static int first_plus_last(char *s) { return s[0] + s[10]; }

int main(void) {
  char *message = "write error";
  printf("%d\n", first_plus_last(message));
  return 0;
}
// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: c"write error".as_ptr() as *mut i8
// REWRITES-NOT: b"write error\0".as_ptr()
// REWRITES: {{^}}}
