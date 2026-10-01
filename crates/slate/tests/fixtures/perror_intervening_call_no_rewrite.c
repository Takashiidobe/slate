#include <stdio.h>

int main(void) {
  int rc = remove("slate_perror_intervening_missing.tmp");
  fflush(stdout);
  if (rc < 0) {
    perror("remove failed");
    return 1;
  }
  return 0;
}
