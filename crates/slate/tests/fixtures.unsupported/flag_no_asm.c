/* { dg-options "-std=gnu17 -fno-asm" } */
int printf(const char *, ...);

int main(void) {
  int asm = 1;
  int typeof = 2;
  printf("%d %d\n", asm, typeof);
  return 0;
}
