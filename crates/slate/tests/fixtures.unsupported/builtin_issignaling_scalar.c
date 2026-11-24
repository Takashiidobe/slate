static int signal32(float value) {
  return __builtin_issignaling(value);
}

static int signal64(double value) {
  return __builtin_issignaling(value);
}

int main(void) {
  return !signal32(__builtin_nansf("")) || signal32(__builtin_nanf("")) ||
         signal32(__builtin_inff()) || !signal64(__builtin_nans("")) ||
         signal64(__builtin_nan("")) || signal64(__builtin_inf());
}
