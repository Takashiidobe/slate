long double          cld_real(long double _Complex z);
long double          cld_imag(long double _Complex z);
long double _Complex cld_conj(long double _Complex z);
long double _Complex cld_scale(long double _Complex z, long double k, int n);

int conj_scaled_sum(int n) {
  long double _Complex z = cld_conj(cld_scale(1.5L + 0.25iL, 2.0L, n));
  return (int)(cld_real(z) * 100 + cld_imag(z) * 10);
}
