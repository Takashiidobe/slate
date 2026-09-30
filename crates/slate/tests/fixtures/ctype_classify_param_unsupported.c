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
