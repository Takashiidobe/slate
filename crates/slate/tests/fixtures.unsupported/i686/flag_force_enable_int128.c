/* { dg-options "-fforce-enable-int128" } */
int printf(const char *, ...);

int main(void) {
  unsigned __int128 big = (unsigned __int128)1 << 100;
  __int128 negative = -(__int128)big;
  big += 12345;
  printf("%zu %llx %llx\n", sizeof big, (unsigned long long)(big >> 64), (unsigned long long)big);
  printf("%d %llx\n", negative < 0, (unsigned long long)((unsigned __int128)negative >> 96));
  return 0;
}
