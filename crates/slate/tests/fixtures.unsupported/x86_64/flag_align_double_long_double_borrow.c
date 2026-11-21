/* { dg-options "-malign-double" } */
int printf(const char *, ...);
struct extended { char c; long double x; };
static void half(long double *p) { *p /= 2; }
int main(void) {
  struct extended e = {1, 5.0L};
  half(&e.x);
  printf("%d\n", (int)e.x);
  return 0;
}
