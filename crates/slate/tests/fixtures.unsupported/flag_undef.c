/* { dg-options "-undef" } */
int printf(const char *, ...);

int main(void) {
#if defined(__linux__) || defined(linux) || defined(__x86_64__) || defined(__GNUC__) || defined(__clang__)
  int target_macros = 1;
#else
  int target_macros = 0;
#endif
#ifdef __STDC_VERSION__
  int stdc = 1;
#else
  int stdc = 0;
#endif
  printf("%d %d\n", target_macros, stdc);
  return 0;
}
