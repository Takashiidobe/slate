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




// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %0 @nexttowardf128(%1 from: f128, %2 toward: f128) -> f128 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f128, signature=fn(f128, f128) -> f128>(__builtin_nextafterf128, read<f128>(%1), read<f128>(%2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 value: f128 [storage=automatic] = call<f128, signature=fn(f128) -> f128>(__builtin_elementwise_sqrt, const<f128>(1));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128) -> f128>(__builtin_acoshf128, read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(__builtin_acoshf128, read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128) -> f128>(__builtin_asinhf128, read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(__builtin_asinhf128, read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128) -> f128>(__builtin_atanhf128, read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(__builtin_atanhf128, read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128) -> f128>(__builtin_cbrtf128, read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(__builtin_cbrtf128, read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128, f128) -> f128>(__builtin_copysignf128, read<f128>(%4), read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(__builtin_copysignf128, read<f128>(%4), read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128) -> f128>(__builtin_erff128, read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(__builtin_erff128, read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128) -> f128>(__builtin_erfcf128, read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(__builtin_erfcf128, read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128) -> f128>(__builtin_expm1f128, read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(__builtin_expm1f128, read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128, f128) -> f128>(__builtin_fdimf128, read<f128>(%4), read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(__builtin_fdimf128, read<f128>(%4), read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128) -> f128>(__builtin_fabsf128, read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(__builtin_fabsf128, read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128, f128) -> f128>(__builtin_hypotf128, read<f128>(%4), read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(__builtin_hypotf128, read<f128>(%4), read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128) -> f128>(__builtin_lgammaf128, read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(__builtin_lgammaf128, read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128) -> f128>(__builtin_log1pf128, read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(__builtin_log1pf128, read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128) -> f128>(__builtin_nearbyintf128, read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(__builtin_nearbyintf128, read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128, f128) -> f128>(__builtin_nextafterf128, read<f128>(%4), read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(__builtin_nextafterf128, read<f128>(%4), read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128, f128) -> f128>(__builtin_nexttowardf128, read<f128>(%4), read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(__builtin_nexttowardf128, read<f128>(%4), read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128, f128) -> f128>(__builtin_remainderf128, read<f128>(%4), read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(__builtin_remainderf128, read<f128>(%4), read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128, i64) -> f128>(__builtin_scalblnf128, read<f128>(%4), widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, i64) -> f128>(__builtin_scalblnf128, read<f128>(%4), widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128, i32) -> f128>(__builtin_scalbnf128, read<f128>(%4), const<i32>(0)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, i32) -> f128>(__builtin_scalbnf128, read<f128>(%4), const<i32>(0));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %5 integral: f128 [storage=automatic];
// DEFAULT-NEXT:             let %6 quotient: i32 [storage=automatic];
// DEFAULT-NEXT:             write<f128>(%4, call<f128, signature=fn(f128, ptr<f128>) -> f128>(__builtin_modff128, read<f128>(%4), addr_of<ptr<f128>>(%5)));
// DEFAULT-NEXT:             call<f128, signature=fn(f128, ptr<f128>) -> f128>(__builtin_modff128, read<f128>(%4), addr_of<ptr<f128>>(%5));
// DEFAULT-NEXT:             write<f128>(%4, call<f128, signature=fn(f128, f128, ptr<i32>) -> f128>(__builtin_remquof128, read<f128>(%4), read<f128>(%4), addr_of<ptr<i32>>(%6)));
// DEFAULT-NEXT:             call<f128, signature=fn(f128, f128, ptr<i32>) -> f128>(__builtin_remquof128, read<f128>(%4), read<f128>(%4), addr_of<ptr<i32>>(%6));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<i32, signature=fn(f128) -> i32>(__builtin_ilogbf128, const<f128>(1));
// DEFAULT-NEXT:         call<i64, signature=fn(f128) -> i64>(__builtin_llrintf128, const<f128>(1));
// DEFAULT-NEXT:         call<i64, signature=fn(f128) -> i64>(__builtin_llroundf128, const<f128>(1));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(__builtin_logbf128, const<f128>(1));
// DEFAULT-NEXT:         call<i64, signature=fn(f128) -> i64>(__builtin_lrintf128, const<f128>(1));
// DEFAULT-NEXT:         call<i64, signature=fn(f128) -> i64>(__builtin_lroundf128, const<f128>(1));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128) -> f128>(__builtin_tgammaf128, read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(__builtin_tgammaf128, read<f128>(%4));
// DEFAULT-NEXT:         if ne<f128, exceptions=ignore>(read<f128>(%4), const<f128>(1))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
