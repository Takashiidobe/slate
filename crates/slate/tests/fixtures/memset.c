#include <stdio.h>
#include <string.h>

static int get_count(void) { return 4; }

int main(void) {
  unsigned char zero_buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  memset(zero_buf, 0, sizeof(zero_buf));

  unsigned char value_buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  memset(value_buf, 0x41, sizeof(value_buf));

  unsigned char partial_buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  memset(partial_buf, 9, 4);

  unsigned char dynamic_buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  int n = get_count();
  memset(dynamic_buf, 9, n);

  for (int i = 0; i < 8; i++)
    printf("%d ", zero_buf[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", value_buf[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", partial_buf[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", dynamic_buf[i]);
  printf("\n");
  return 0;
}
