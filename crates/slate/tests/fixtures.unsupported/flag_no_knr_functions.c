/* { dg-options "-std=c17 -fno-knr-functions" } */
int printf(const char *, ...);

int answer();

int main(void) {
  int prototyped = _Generic(&answer, int (*)(int): 0, int (*)(void): 1, default: 2);
  printf("%d %d\n", prototyped, answer());
  return 0;
}

int answer(void) { return 42; }
