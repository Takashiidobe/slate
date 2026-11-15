#include <stdio.h>

extern const unsigned char crypto_sbox[4];
extern int table_alias[3];
extern int weak_table[3];
extern int table[3];
int sbox_at(int i);

int main(void) {
  table_alias[1] = 21;
  printf("%d %d %d %d\n", crypto_sbox[2], sbox_at(3), table[1], weak_table[2]);
  return 0;
}
