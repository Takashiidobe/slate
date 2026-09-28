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
// DEFAULT-NEXT:     fn %9 @__builtin_nextafterf128(%7 <unnamed>: f128, %8 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %0 @nexttowardf128(%1 from: f128, %2 toward: f128) -> f128 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f128, signature=fn(f128, f128) -> f128>(%9, read<f128>(%1), read<f128>(%2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @__builtin_elementwise_sqrt(%10 <unnamed>: f128) -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %13 @__builtin_acoshf128(%12 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %15 @__builtin_asinhf128(%14 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %17 @__builtin_atanhf128(%16 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %19 @__builtin_cbrtf128(%18 <unnamed>: f128) -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %22 @__builtin_copysignf128(%20 <unnamed>: f128, %21 <unnamed>: f128) -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %24 @__builtin_erff128(%23 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %26 @__builtin_erfcf128(%25 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %28 @__builtin_expm1f128(%27 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %31 @__builtin_fdimf128(%29 <unnamed>: f128, %30 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %33 @__builtin_fabsf128(%32 <unnamed>: f128) -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %36 @__builtin_hypotf128(%34 <unnamed>: f128, %35 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %38 @__builtin_lgammaf128(%37 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %40 @__builtin_log1pf128(%39 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %42 @__builtin_nearbyintf128(%41 <unnamed>: f128) -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %45 @__builtin_nexttowardf128(%43 <unnamed>: f128, %44 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %48 @__builtin_remainderf128(%46 <unnamed>: f128, %47 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %51 @__builtin_scalblnf128(%49 <unnamed>: f128, %50 <unnamed>: i64) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %54 @__builtin_scalbnf128(%52 <unnamed>: f128, %53 <unnamed>: i32) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %57 @__builtin_modff128(%55 <unnamed>: f128, %56 <unnamed>: ptr<f128>) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %61 @__builtin_remquof128(%58 <unnamed>: f128, %59 <unnamed>: f128, %60 <unnamed>: ptr<i32>) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %63 @__builtin_ilogbf128(%62 <unnamed>: f128) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %65 @__builtin_llrintf128(%64 <unnamed>: f128) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %67 @__builtin_llroundf128(%66 <unnamed>: f128) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %69 @__builtin_logbf128(%68 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %71 @__builtin_lrintf128(%70 <unnamed>: f128) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %73 @__builtin_lroundf128(%72 <unnamed>: f128) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %75 @__builtin_tgammaf128(%74 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 value: f128 [storage=automatic] = call<f128, signature=fn(f128) -> f128>(%11, const<f128>(1));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128) -> f128>(%13, read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%13, read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128) -> f128>(%15, read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%15, read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128) -> f128>(%17, read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%17, read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128) -> f128>(%19, read<f128>(%4)));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128, f128) -> f128>(%22, read<f128>(%4), read<f128>(%4)));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128) -> f128>(%24, read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%24, read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128) -> f128>(%26, read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%26, read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128) -> f128>(%28, read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%28, read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128, f128) -> f128>(%31, read<f128>(%4), read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(%31, read<f128>(%4), read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128) -> f128>(%33, read<f128>(%4)));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128, f128) -> f128>(%36, read<f128>(%4), read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(%36, read<f128>(%4), read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128) -> f128>(%38, read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%38, read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128) -> f128>(%40, read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%40, read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128) -> f128>(%42, read<f128>(%4)));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128, f128) -> f128>(%9, read<f128>(%4), read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(%9, read<f128>(%4), read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128, f128) -> f128>(%45, read<f128>(%4), read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(%45, read<f128>(%4), read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128, f128) -> f128>(%48, read<f128>(%4), read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(%48, read<f128>(%4), read<f128>(%4));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128, i64) -> f128>(%51, read<f128>(%4), widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, i64) -> f128>(%51, read<f128>(%4), widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128, i32) -> f128>(%54, read<f128>(%4), const<i32>(0)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, i32) -> f128>(%54, read<f128>(%4), const<i32>(0));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %5 integral: f128 [storage=automatic];
// DEFAULT-NEXT:             let %6 quotient: i32 [storage=automatic];
// DEFAULT-NEXT:             write<f128>(%4, call<f128, signature=fn(f128, ptr<f128>) -> f128>(%57, read<f128>(%4), addr_of<ptr<f128>>(%5)));
// DEFAULT-NEXT:             call<f128, signature=fn(f128, ptr<f128>) -> f128>(%57, read<f128>(%4), addr_of<ptr<f128>>(%5));
// DEFAULT-NEXT:             write<f128>(%4, call<f128, signature=fn(f128, f128, ptr<i32>) -> f128>(%61, read<f128>(%4), read<f128>(%4), addr_of<ptr<i32>>(%6)));
// DEFAULT-NEXT:             call<f128, signature=fn(f128, f128, ptr<i32>) -> f128>(%61, read<f128>(%4), read<f128>(%4), addr_of<ptr<i32>>(%6));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<i32, signature=fn(f128) -> i32>(%63, const<f128>(1));
// DEFAULT-NEXT:         call<i64, signature=fn(f128) -> i64>(%65, const<f128>(1));
// DEFAULT-NEXT:         call<i64, signature=fn(f128) -> i64>(%67, const<f128>(1));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%69, const<f128>(1));
// DEFAULT-NEXT:         call<i64, signature=fn(f128) -> i64>(%71, const<f128>(1));
// DEFAULT-NEXT:         call<i64, signature=fn(f128) -> i64>(%73, const<f128>(1));
// DEFAULT-NEXT:         write<f128>(%4, call<f128, signature=fn(f128) -> f128>(%75, read<f128>(%4)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%75, read<f128>(%4));
// DEFAULT-NEXT:         if ne<f128, exceptions=ignore>(read<f128>(%4), const<f128>(1))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
