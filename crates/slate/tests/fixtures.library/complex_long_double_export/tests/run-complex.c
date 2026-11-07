extern long double          cld_real(long double _Complex z);
extern long double          cld_imag(long double _Complex z);
extern long double _Complex cld_conj(long double _Complex z);
extern long double _Complex cld_scale(long double _Complex z, long double k, int n);
extern int                  conj_scaled_sum(int n);

int main(void) {
  long double _Complex z = 3.0L + 0.1iL;
  if (cld_real(z) != 3.0L || cld_imag(z) != 0.1L)
    return 1;
  long double _Complex c = cld_conj(z);
  if (__real__ c != 3.0L || __imag__ c != -0.1L)
    return 2;
  long double _Complex s = cld_scale(z, 2.0L, 3);
  if (__real__ s != 24.0L || __imag__ s != 0.8L)
    return 3;
  return conj_scaled_sum(2) == 590 ? 0 : 4;
}
