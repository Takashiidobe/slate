void abort(void);
void exit(int);

long double ll_to_ld(long long n) { return n; }

long long ld_to_ll(long double n) { return n; }

int main(void) {
  long long n;

  if (ll_to_ld(10LL) != 10.0)
    abort();

  if (ld_to_ll(10.0) != 10)
    abort();

  exit(0);
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%8 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @ll_to_ld(%3 n: i64) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<f80, reason=return, exact=true, rounding=nearest_even, exceptions=ignore>(read<i64>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @ld_to_ll(%5 n: f80) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i64, reason=return, out_of_range=ub, exceptions=ignore>(read<f80>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 n: i64 [storage=automatic];
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(call<f80, signature=fn(i64) -> f80>(%2, const<i64>(10)), float_widen<f80, reason=usual_arith>(const<f64>(10.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(f80) -> i64>(%4, float_widen<f80, reason=arg>(const<f64>(10.0))), widen<i64, reason=usual_arith>(const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
