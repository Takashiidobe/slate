static const unsigned char sbox[4] = {1, 2, 3, 4};
extern const unsigned char crypto_sbox[4] __attribute__((alias("sbox")));

int table[3] = {10, 20, 30};
extern int table_alias[3] __attribute__((alias("table")));
extern int weak_table[3] __attribute__((weak, alias("table")));

int sbox_at(int i) { return sbox[i]; }
