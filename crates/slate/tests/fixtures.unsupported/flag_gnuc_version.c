/* { dg-options "-fgnuc-version=5.4.3" } */
int printf(const char *, ...);

int main(void) {
  printf("%d %d %d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
  return 0;
}
