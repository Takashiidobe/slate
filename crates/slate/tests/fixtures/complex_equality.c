#include <stdio.h>

int main(void) {
  float _Complex f       = __builtin_complex(1.0f, 2.0f);
  double _Complex d      = __builtin_complex(3.0, 4.0);
  long double _Complex l = __builtin_complex(5.0L, 6.0L);

  printf("%d %d\n", f == __builtin_complex(1.0f, 2.0f),
         f != __builtin_complex(1.0f, 3.0f));
  printf("%d %d\n", d == __builtin_complex(3.0, 4.0),
         d != __builtin_complex(3.0, 5.0));
  printf("%d %d\n", l == __builtin_complex(5.0L, 6.0L),
         l != __builtin_complex(5.0L, 7.0L));
  return 0;
}
