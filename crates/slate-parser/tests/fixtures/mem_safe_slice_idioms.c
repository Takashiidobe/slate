#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int get_n(void) { return 3; }

int main(void) {
  unsigned char dst_a[8] = {0};
  unsigned char src_a[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  memcpy(dst_a, src_a, 4);

  unsigned char dst_b[8] = {9, 9, 9, 9, 9, 9, 9, 9};
  unsigned char src_b[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  memmove(dst_b, src_b, 5);

  unsigned char self_c[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  memmove(self_c, self_c, 8);

  unsigned char dst_d[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  memset(dst_d, 0x41, 5);

  unsigned char dst_e[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  unsigned char src_e[8] = {9, 9, 9, 9, 9, 9, 9, 9};
  memcpy(dst_e, src_e + 2, 4);

  unsigned char dst_f[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  unsigned char src_f[8] = {9, 9, 9, 9, 9, 9, 9, 9};
  int           n        = get_n();
  memcpy(dst_f, src_f, n);

  unsigned char  dst_g[4] = {0, 0, 0, 0};
  unsigned char *src_g    = malloc(4);
  memset(src_g, 5, 4);
  memcpy(dst_g, src_g, 4);
  free(src_g);

  for (int i = 0; i < 8; i++)
    printf("%d ", dst_a[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", dst_b[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", self_c[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", dst_d[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", dst_e[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", dst_f[i]);
  for (int i = 0; i < 4; i++)
    printf("%d ", dst_g[i]);
  printf("\n");
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
