/* { dg-options "--flavor=gcc -fexec-charset=IBM1047" } */
int putchar(int);

static void put_hex(unsigned value) {
  unsigned high = value >> 4, low = value & 15;
  putchar(high < 10 ? 48 + high : 87 + high);
  putchar(low < 10 ? 48 + low : 87 + low);
  putchar(32);
}

int main(void) {
  const char text[] = "Az09 !";
  for (unsigned i = 0; i < sizeof text; i++)
    put_hex((unsigned char)text[i]);
  put_hex((unsigned char)'a');
  put_hex((unsigned char)'\n');
  putchar(10);
  return 0;
}
