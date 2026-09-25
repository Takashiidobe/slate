/* { dg-do run }

   # AVR doubles are floats
   { dg-skip-if "AVR doubles are floats" { avr-*-* } }
   { dg-skip-if "AVR doubles are floats" { d10v-*-* } { "*" } { "*-mdouble64*" }
   } */
#if defined(__i386__) && defined(__FreeBSD__)
#include <ieeefp.h>
#endif

void abort(void);
void exit(int);

double d(unsigned long long k) {
  double x;

  x = (double)k;
  return x;
}

float s(unsigned long long k) {
  float x;

  x = (float)k;
  return x;
}

int main(void) {
  unsigned long long int k;
  double                 x;

#if defined(__i386__) && defined(__FreeBSD__)
  /* This test case assumes extended-precision, but FreeBSD defaults to
     double-precision.  Make it so.  */
  fpsetprec(FP_PE);
#endif

  if (sizeof(double) >= 8) {
    k = 0x8693ba6d7d220401ULL;
    x = d(k);
    k = (unsigned long long)x;
    if (k != 0x8693ba6d7d220800ULL)
      abort();
  }

  k = 0x8234508000000001ULL;
  x = s(k);
  k = (unsigned long long)x;
  if (k != 0x8234510000000000ULL)
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
// DEFAULT-NEXT:     fn %1 @exit(%11 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @d(%3 k: u64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 x: f64 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(%4, int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<u64>(%3)));
// DEFAULT-NEXT:         return read<f64>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @s(%6 k: u64) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 x: f32 [storage=automatic];
// DEFAULT-NEXT:         write<f32>(%7, int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<u64>(%6)));
// DEFAULT-NEXT:         return read<f32>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 k: u64 [storage=automatic];
// DEFAULT-NEXT:         let %10 x: f64 [storage=automatic];
// DEFAULT-NEXT:         if ge<u64>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<u64>(%9, const<u64>(9697299402072392705));
// DEFAULT-NEXT:                 write<f64>(%10, call<f64, signature=fn(u64) -> f64>(%2, read<u64>(%9)));
// DEFAULT-NEXT:                 call<f64, signature=fn(u64) -> f64>(%2, read<u64>(%9));
// DEFAULT-NEXT:                 write<u64>(%9, float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(%10)));
// DEFAULT-NEXT:                 if ne<u64>(read<u64>(%9), const<u64>(9697299402072393728))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<u64>(%9, const<u64>(9382212434405621761));
// DEFAULT-NEXT:         write<f64>(%10, float_widen<f64, reason=assign>(call<f32, signature=fn(u64) -> f32>(%5, read<u64>(%9))));
// DEFAULT-NEXT:         float_widen<f64, reason=assign>(call<f32, signature=fn(u64) -> f32>(%5, read<u64>(%9)));
// DEFAULT-NEXT:         write<u64>(%9, float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(%10)));
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%9), const<u64>(9382212984161435648))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
