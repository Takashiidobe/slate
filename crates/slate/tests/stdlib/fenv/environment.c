#include <fenv.h>
#include <stdio.h>

int main(void) {
  fenv_t original_environment;
  fenv_t saved_environment;
  fenv_t held_environment;

  printf("fenv_t %zu\n", sizeof(fenv_t));
  printf("fegetenv original %d\n", fegetenv(&original_environment));
  printf("feclearexcept initial %d\n", feclearexcept(FE_ALL_EXCEPT));
  printf("fegetenv saved %d\n", fegetenv(&saved_environment));
  printf("feraiseexcept before setenv %d\n", feraiseexcept(FE_INVALID));
  printf("fesetenv saved %d\n", fesetenv(&saved_environment));
  printf("fesetenv flags %d\n", fetestexcept(FE_ALL_EXCEPT));

  printf("feraiseexcept before hold %d\n", feraiseexcept(FE_DIVBYZERO));
  printf("feholdexcept %d\n", feholdexcept(&held_environment));
  printf("feholdexcept flags %d\n", fetestexcept(FE_ALL_EXCEPT));
  printf("feraiseexcept held %d\n", feraiseexcept(FE_OVERFLOW));
  printf("feupdateenv %d\n", feupdateenv(&held_environment));
  printf("feupdateenv flags %d\n",
         fetestexcept(FE_DIVBYZERO | FE_OVERFLOW) ==
             (FE_DIVBYZERO | FE_OVERFLOW));

  printf("fesetenv default %d\n", fesetenv(FE_DFL_ENV));
  printf("default environment %d %d\n", fegetround() == FE_TONEAREST,
         fetestexcept(FE_ALL_EXCEPT));
  printf("fesetenv original %d\n", fesetenv(&original_environment));
  return 0;
}
