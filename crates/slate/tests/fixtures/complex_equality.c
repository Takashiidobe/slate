#include <stdio.h>

int main(void) {
  float _Complex f = __builtin_complex(1.0f, 2.0f);
  double _Complex d = __builtin_complex(3.0, 4.0);
  long double l_re = 5.0L;
  long double l_im = 6.0L;
  long double other_l_im = 7.0L;
  long double _Complex l = __builtin_complex(l_re, l_im);
  long double _Complex same_l = __builtin_complex(l_re, l_im);
  long double _Complex other_l = __builtin_complex(l_re, other_l_im);

  printf("%d %d\n", f == __builtin_complex(1.0f, 2.0f),
         f != __builtin_complex(1.0f, 3.0f));
  printf("%d %d\n", d == __builtin_complex(3.0, 4.0),
         d != __builtin_complex(3.0, 5.0));
  printf("%d %d\n", l == same_l, l != other_l);
  return 0;
}
