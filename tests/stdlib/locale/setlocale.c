#include <locale.h>
#include <stdio.h>
#include <string.h>
int main(void) {
  char *s = setlocale(LC_ALL, "C");
  char *q = setlocale(LC_ALL, NULL);
  printf("%d %d\n", s != NULL, q != NULL && strcmp(q, "") != 0);
  return 0;
}
