#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

int main(void) {
  /* flags */
  printf("%d|%-d|%+d|%05d|%#x|%#o|% d\n", -3, 5, 5, 5, 255, 8, 5);

  /* width */
  printf("%5d|%-5d|%05d\n", 42, 42, 42);
  printf("%*d\n", 5, 42);

  /* precision */
  printf("%.3d|%8.3d|%-8.3d\n", 5, 5, 5);
  printf("%.5s\n", "hello world");
  printf("%.*d\n", 3, 5);

  /* length modifiers */
  short sh = 7;
  signed char sc = 8;
  long lo = 9L;
  long long ll = 10LL;
  intmax_t jm = 11;
  size_t zu = 12u;
  ssize_t zd = -12;
  ptrdiff_t td = 13;
  long double ld = 1.5L;
  printf("%hd|%hhd|%ld|%lld|%jd|%zu|%zd|%td|%Lf\n", sh, sc, lo, ll, jm, zu, zd,
         td, ld);

  /* conversion specifiers */
  printf("%d %i %u %o %x %X\n", -1, -1, 1u, 8u, 255u, 255u);
  printf("%f %F %e %E %g %G\n", 3.5, 3.5, 12345.6789, 12345.6789, 0.0001234,
         123456.789);
  printf("%a %A\n", 1.0, 1.0);
  printf("%c %s %p\n", 'x', "hi", (void *)&sh);
  printf("100%%\n");

  return 0;
}
