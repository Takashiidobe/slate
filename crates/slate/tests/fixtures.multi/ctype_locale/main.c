#include <ctype.h>
#include <stdio.h>

void set_project_locale(void);

int main(void) {
  set_project_locale();
  int lower = 'q';
  int upper = 'Q';
  printf("%d %d\n", toupper(lower), tolower(upper));
  return 0;
}
// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: unsafe { toupper(
// REWRITES-DAG: unsafe { tolower(
// REWRITES: {{^}}}
