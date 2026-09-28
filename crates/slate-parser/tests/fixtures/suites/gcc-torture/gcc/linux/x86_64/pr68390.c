/* { dg-do run }  */
/* { dg-options "-O2" } */

__attribute__((noinline)) double direct(int x, ...) { return x * x; }

__attribute__((noinline)) double broken(double (*indirect)(int x, ...), int v) {
  return indirect(v);
}

int main() {
  double d1, d2;
  int    i = 2;
  d1       = broken(direct, i);
  if (d1 != i * i) {
    __builtin_abort();
  }
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
// DEFAULT-NEXT:     fn %0 @direct(%1 x: i32, ...) -> f64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<f64, reason=return, exact=true, rounding=nearest_even, exceptions=observable>(mul<i32, overflow=ub>(read<i32>(%1), read<i32>(%1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @broken(%3 indirect: ptr<fn(i32, ...) -> f64>, %4 v: i32) -> f64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(i32, ...) -> f64>(read<ptr<fn(i32, ...) -> f64>>(%3), read<i32>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 d1: f64 [storage=automatic];
// DEFAULT-NEXT:         let %7 d2: f64 [storage=automatic];
// DEFAULT-NEXT:         let %8 i: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         write<f64>(%6, call<f64, signature=fn(ptr<fn(i32, ...) -> f64>, i32) -> f64>(%2, function_decay<ptr<fn(i32, ...) -> f64>>(%0), read<i32>(%8)));
// DEFAULT-NEXT:         call<f64, signature=fn(ptr<fn(i32, ...) -> f64>, i32) -> f64>(%2, function_decay<ptr<fn(i32, ...) -> f64>>(%0), read<i32>(%8));
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(read<f64>(%6), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(mul<i32, overflow=ub>(read<i32>(%8), read<i32>(%8))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
