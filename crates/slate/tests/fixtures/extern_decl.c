#include <stdio.h>

int toupper(int c);
int tolower(int c);
int abs(int n);

int main(void) {
  printf("%c%c %d\n", toupper('a'), tolower('Z'), abs(-42));
  return 0;
}
// REWRITES-DAG: unsafe { toupper(
// REWRITES-DAG: as i32) }
// REWRITES-DAG: as u8) as char
// REWRITES-NOT: unsafe { printf(
