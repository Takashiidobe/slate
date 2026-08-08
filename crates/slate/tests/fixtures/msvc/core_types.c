#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

int classify_div(int numerator, int denominator) {
  div_t result = div(numerator, denominator);
  return result.quot + result.rem;
}

long classify_ldiv(long numerator, long denominator) {
  ldiv_t result = ldiv(numerator, denominator);
  return result.quot + result.rem;
}

long long classify_lldiv(long long numerator, long long denominator) {
  lldiv_t result = lldiv(numerator, denominator);
  return result.quot + result.rem;
}

int last_error_is_invalid(void) { return errno == EINVAL; }

int year_from_time(time_t when) {
  struct tm *parts = localtime(&when);
  return parts->tm_year + 1900;
}

static volatile sig_atomic_t last_signal;
void record_signal(int sig) { last_signal = sig; }

void install_handler(void) { signal(SIGABRT, record_signal); }

int main(void) { return 0; }
