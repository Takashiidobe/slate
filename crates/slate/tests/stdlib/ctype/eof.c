#include <ctype.h>
#include <stdio.h>
int main(void) {
  int volatile e = EOF;
  printf("%d %d %d\n", isalpha(e) ? 1 : 0, isdigit(e) ? 1 : 0,
         tolower(e) == EOF ? 1 : 0);
  return 0;
}
