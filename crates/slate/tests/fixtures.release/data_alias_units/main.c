#include <stdio.h>

extern const unsigned char crypto_sbox[4];
extern int table_alias[3];
extern int weak_table[3];
extern int table[3];
int sbox_at(int i);
extern const void *alias_refs[2];
extern const void *alias_operand_refs[2];
void emit_alias_operand_refs(void);

int main(void) {
  emit_alias_operand_refs();
  table_alias[1] = 21;
  printf("%d %d %d %d\n", crypto_sbox[2], sbox_at(3), table[1], weak_table[2]);
  printf("%d %d\n", alias_refs[0] == crypto_sbox, alias_refs[1] == table_alias);
  printf("%d %d\n", alias_operand_refs[0] == crypto_sbox,
         alias_operand_refs[1] == table_alias);
  return 0;
}
