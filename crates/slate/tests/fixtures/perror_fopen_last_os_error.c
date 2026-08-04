#include <stdio.h>

int main(void) {
  FILE *fp = fopen("slate_perror_fopen_missing.tmp", "r");
  if (fp == NULL) {
    perror("open failed");
    return 1;
  }
  fclose(fp);
  return 0;
}
