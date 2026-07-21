#include "test.h"

void *malloc(long size);
void *realloc(void *ptr, long size);
void free(void *ptr);

int main() {
  ASSERT(2, ({
           int *p = malloc(2 * sizeof(int));
           p[0] = 1;
           p[1] = 2;
           int v = p[1];
           free(p);
           v;
         }));

  ASSERT(3, ({
           int *p = malloc(2 * sizeof(int));
           p[0] = 1;
           p[1] = 2;
           p = realloc(p, 3 * sizeof(int));
           p[2] = 3;
           int v = p[2];
           free(p);
           v;
         }));

  ASSERT(7, ({
           int *p = malloc(3 * sizeof(int));
           p[0] = 7;
           p[1] = 8;
           p[2] = 9;
           p = realloc(p, 2 * sizeof(int));
           int v = p[0];
           free(p);
           v;
         }));

  printf("OK\n");
  return 0;
}
