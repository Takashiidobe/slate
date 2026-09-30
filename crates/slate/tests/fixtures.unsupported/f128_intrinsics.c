static __float128 nexttowardf128(__float128 from, __float128 toward) {
  return __builtin_nextafterf128(from, toward);
}

int main(void) {
  __float128 value = __builtin_elementwise_sqrt(1.0Q);
  value            = __builtin_acoshf128(value);
  value            = __builtin_asinhf128(value);
  value            = __builtin_atanhf128(value);
  value            = __builtin_cbrtf128(value);
  value            = __builtin_copysignf128(value, value);
  value            = __builtin_erff128(value);
  value            = __builtin_erfcf128(value);
  value            = __builtin_expm1f128(value);
  value            = __builtin_fdimf128(value, value);
  value            = __builtin_fabsf128(value);
  value            = __builtin_hypotf128(value, value);
  value            = __builtin_lgammaf128(value);
  value            = __builtin_log1pf128(value);
  value            = __builtin_nearbyintf128(value);
  value            = __builtin_nextafterf128(value, value);
  value            = __builtin_nexttowardf128(value, value);
  value            = __builtin_remainderf128(value, value);
  value            = __builtin_scalblnf128(value, 0);
  value            = __builtin_scalbnf128(value, 0);
  {
    __float128 integral;
    int        quotient;
    value = __builtin_modff128(value, &integral);
    value = __builtin_remquof128(value, value, &quotient);
  }
  (void)__builtin_ilogbf128(1.0Q);
  (void)__builtin_llrintf128(1.0Q);
  (void)__builtin_llroundf128(1.0Q);
  (void)__builtin_logbf128(1.0Q);
  (void)__builtin_lrintf128(1.0Q);
  (void)__builtin_lroundf128(1.0Q);
  value = __builtin_tgammaf128(value);
  if (value != 1.0Q)
    return 1;
  return 0;
}
