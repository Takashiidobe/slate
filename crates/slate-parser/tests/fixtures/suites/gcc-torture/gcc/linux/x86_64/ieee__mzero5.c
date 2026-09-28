/* { dg-do run } */
/* Test gcse handling of IEEE 0/-0 rules.  */
void          abort(void);
static double zero = 0.0;

int negzero_check(double d) {
  if (d == 0)
    return !!__builtin_memcmp((void *)&zero, (void *)&d, sizeof(double));
  return 0;
}

int sub(double d, double e) {
  if (d == 0.0 && e == 0.0 && negzero_check(d) == 0 && negzero_check(e) == 0)
    return 1;
  else
    return 0;
}

int main(void) {
  double minus_zero = -0.0;
  if (sub(minus_zero, 0))
    abort();
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
// DEFAULT-NEXT:     global %1 zero: f64 [storage=static] = const<f64>(0.0) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %12 @__builtin_memcmp(%9 <unnamed>: ptr<const void>, %10 <unnamed>: ptr<const void>, %11 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @negzero_check(%3 d: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<f64, exceptions=observable>(read<f64>(%3), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:             return from_bool<i32, reason=return>(not<bool>(not<bool>(ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%12, pointer_cast<ptr<const void>, reason=arg>(pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<f64>>(%1))), pointer_cast<ptr<const void>, reason=arg>(pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<f64>>(%3))), const<u64>(8)), const<i32>(0)))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @sub(%5 d: f64, %6 e: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %13: bool [synthetic];
// DEFAULT-NEXT:         if logical_and<bool>(eq<f64, exceptions=observable>(read<f64>(%5), const<f64>(0.0)), eq<f64, exceptions=observable>(read<f64>(%6), const<f64>(0.0)))
// DEFAULT-NEXT:             write<bool>(%13, eq<i32>(call<i32, signature=fn(f64) -> i32>(%2, read<f64>(%5)), const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%13, const<bool>(false));
// DEFAULT-NEXT:         let %14: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%13)
// DEFAULT-NEXT:             write<bool>(%14, eq<i32>(call<i32, signature=fn(f64) -> i32>(%2, read<f64>(%6)), const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%14, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%14)
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 minus_zero: f64 [storage=automatic] = neg<f64>(const<f64>(0.0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f64, f64) -> i32>(%4, read<f64>(%8), int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
