#include <string.h>

int main(void) {
  char  buf[8];
  char *p = buf;
  memset((void *)p, 0, 8);
  return p[0];
}


