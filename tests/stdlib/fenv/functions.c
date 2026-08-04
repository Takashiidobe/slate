#include <fenv.h>
#include <stdio.h>

static void print_rounding_mode(const char *name, int mode) {
  int set_status = fesetround(mode);
  printf("%s %d %d\n", name, set_status, fegetround() == mode);
}

int main(void) {
  fexcept_t saved_flags;

  printf("version %ld\n", __STDC_VERSION_FENV_H__);
  printf("fexcept_t %zu\n", sizeof(fexcept_t));
  printf("exceptions %d %d %d %d %d %d\n", FE_DIVBYZERO, FE_INEXACT, FE_INVALID,
         FE_OVERFLOW, FE_UNDERFLOW, FE_ALL_EXCEPT);
  printf("rounding %d %d %d %d\n", FE_DOWNWARD, FE_TONEAREST, FE_TOWARDZERO,
         FE_UPWARD);

  int original_round = fegetround();
  printf("fegetround original %d\n", original_round);

  printf("feclearexcept initial %d\n", feclearexcept(FE_ALL_EXCEPT));
  printf("fetestexcept clear %d\n", fetestexcept(FE_ALL_EXCEPT));
  printf("feraiseexcept %d\n", feraiseexcept(FE_INVALID | FE_INEXACT));
  printf("fetestexcept raised %d\n",
         fetestexcept(FE_INVALID | FE_INEXACT) == (FE_INVALID | FE_INEXACT));
  printf("fegetexceptflag %d\n", fegetexceptflag(&saved_flags, FE_ALL_EXCEPT));
  printf("fetestexceptflag %d\n",
         fetestexceptflag(&saved_flags, FE_ALL_EXCEPT) ==
             (FE_INVALID | FE_INEXACT));
  printf("feclearexcept saved %d\n", feclearexcept(FE_ALL_EXCEPT));
  printf("fesetexceptflag %d\n", fesetexceptflag(&saved_flags, FE_ALL_EXCEPT));
  printf("fesetexceptflag flags %d\n",
         fetestexcept(FE_INVALID | FE_INEXACT) == (FE_INVALID | FE_INEXACT));
  printf("feclearexcept set %d\n", feclearexcept(FE_ALL_EXCEPT));
  printf("fesetexcept %d\n", fesetexcept(FE_OVERFLOW));
  printf("fesetexcept flags %d\n", fetestexcept(FE_ALL_EXCEPT) == FE_OVERFLOW);
  printf("feclearexcept finish %d\n", feclearexcept(FE_ALL_EXCEPT));

  print_rounding_mode("FE_DOWNWARD", FE_DOWNWARD);
  print_rounding_mode("FE_TONEAREST", FE_TONEAREST);
  print_rounding_mode("FE_TOWARDZERO", FE_TOWARDZERO);
  print_rounding_mode("FE_UPWARD", FE_UPWARD);
  printf("fesetround restore %d\n", fesetround(original_round));

#ifdef FE_TONEARESTFROMZERO
  print_rounding_mode("FE_TONEARESTFROMZERO", FE_TONEARESTFROMZERO);
#else
  printf("FE_TONEARESTFROMZERO unavailable\n");
#endif
#ifdef __STDC_IEC_60559_DFP__
  printf("decimal %d %d\n", fe_dec_setround(FE_DEC_TONEAREST),
         fe_dec_getround() == FE_DEC_TONEAREST);
#else
  printf("decimal unavailable\n");
#endif
#ifdef FE_SNANS_ALWAYS_SIGNAL
  printf("FE_SNANS_ALWAYS_SIGNAL %d\n", FE_SNANS_ALWAYS_SIGNAL);
#else
  printf("FE_SNANS_ALWAYS_SIGNAL unavailable\n");
#endif

  printf("feclearexcept restore %d\n", feclearexcept(FE_ALL_EXCEPT));
  return 0;
}
