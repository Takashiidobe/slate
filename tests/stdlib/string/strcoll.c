#include <locale.h>
#include <string.h>
#include <stdio.h>
int main(void) {
  setlocale(LC_ALL, "C");
  printf("%d %d %d\n", strcoll("abc", "abc") == 0, strcoll("abc", "abd") < 0, strcoll("abd", "abc") > 0);
  return 0;
}
