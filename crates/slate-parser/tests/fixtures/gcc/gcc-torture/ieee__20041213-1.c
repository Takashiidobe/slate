/* { dg-do run } */
#if defined(__AVR__) && (__SIZEOF_DOUBLE__ == __SIZEOF_FLOAT__)
extern double sqrt(double) __asm("sqrtf");
#else
extern double sqrt(double);
#endif
extern void abort(void);
int         once;

double foo(void) {
  if (once++)
    abort();
  return 0.0 / 0.0;
}

double x;
int    main(void) {
  x = sqrt(foo());
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
// DEFAULT-NEXT:     global %2 once: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 x: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @sqrt(%6 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo() -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %8: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%7), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%8));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%7), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         return div<f64, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0), const<f64>(0.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<f64>(%4, call<f64, signature=fn(f64) -> f64>(%0, call<f64, signature=fn() -> f64>(%3)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%0, call<f64, signature=fn() -> f64>(%3));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
