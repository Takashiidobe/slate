#include <stdio.h>

static void set_data(void *ptr) {
  char *p = (char *)ptr;
  printf("%s\n", p);
}

int main(void) {
  char text[] = "hello";
  set_data(&text);
  return 0;
}
