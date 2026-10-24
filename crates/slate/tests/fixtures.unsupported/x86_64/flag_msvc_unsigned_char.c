/* { dg-options "--flavor=msvc /J" } */
int printf(const char *, ...);

int main(void) {
  char c = (char)200;
  printf("%d %d %d\n", c, c > 0, (char)-1 == 255);
  return 0;
}
