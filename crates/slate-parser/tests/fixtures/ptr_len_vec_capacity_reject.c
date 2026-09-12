#include <stdio.h>
#include <stdlib.h>

static int consume_extra_capacity(int *values, int len) {
  int sum = 0;
  for (int i = 0; i < len; ++i) {
    values[i]  = i + 1;
    sum       += values[i];
  }
  free(values);
  return sum;
}

int main(void) {
  int  len    = 3;
  int *values = malloc((len + 1) * sizeof(int));
  int  sum    = consume_extra_capacity(values, len);
  printf("%d\n", sum);
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
