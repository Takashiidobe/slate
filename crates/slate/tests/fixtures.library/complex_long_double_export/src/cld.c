long double cld_real(long double _Complex z) { return __real__ z; }

long double cld_imag(long double _Complex z) { return __imag__ z; }

long double _Complex cld_conj(long double _Complex z) { return __real__ z - (__imag__ z) * 1.0iL; }

long double _Complex cld_scale(long double _Complex z, long double k, int n) {
  while (n-- > 0)
    z *= k;
  return z;
}
