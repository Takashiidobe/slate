/* { dg-do compile } */
/* { dg-options -Wtraditional-conversion } */

int
test_s (signed int x)
{
  return __builtin_abs (x)	/* { dg-bogus "as unsigned due to prototype" } */
    + __builtin_clz (x)		/* { dg-warning "as unsigned due to prototype" } */
    + __builtin_ctz (x)		/* { dg-warning "as unsigned due to prototype" } */
    + __builtin_clrsb (x)	/* { dg-bogus "as unsigned due to prototype" } */
    + __builtin_ffs (x)		/* { dg-bogus "as unsigned due to prototype" } */
    + __builtin_parity (x)	/* { dg-warning "as unsigned due to prototype" } */
    + __builtin_popcount (x);	/* { dg-warning "as unsigned due to prototype" } */
}

int
test_u (unsigned int x)
{
  return __builtin_abs (x)	/* { dg-warning "as signed due to prototype" } */
    + __builtin_clz (x)		/* { dg-bogus "as signed due to prototype" } */
    + __builtin_ctz (x)		/* { dg-bogus "as signed due to prototype" } */
    + __builtin_clrsb (x)	/* { dg-warning "as signed due to prototype" } */
    + __builtin_ffs (x)		/* { dg-warning "as signed due to prototype" } */
    + __builtin_parity (x)	/* { dg-bogus "as signed due to prototype" } */
    + __builtin_popcount (x);	/* { dg-bogus "as signed due to prototype" } */
}

int
test_sl (signed long x)
{
  return __builtin_labs (x)	/* { dg-bogus "as unsigned due to prototype" } */
    + __builtin_clzl (x)	/* { dg-warning "as unsigned due to prototype" } */
    + __builtin_ctzl (x)	/* { dg-warning "as unsigned due to prototype" } */
    + __builtin_clrsbl (x)	/* { dg-bogus "as unsigned due to prototype" } */
    + __builtin_ffsl (x)	/* { dg-bogus "as unsigned due to prototype" } */
    + __builtin_parityl (x)	/* { dg-warning "as unsigned due to prototype" } */
    + __builtin_popcountl (x);	/* { dg-warning "as unsigned due to prototype" } */
}

int
test_ul (unsigned long x)
{
  return __builtin_labs (x)	/* { dg-warning "as signed due to prototype" } */
    + __builtin_clzl (x)	/* { dg-bogus "as signed due to prototype" } */
    + __builtin_ctzl (x)	/* { dg-bogus "as signed due to prototype" } */
    + __builtin_clrsbl (x)	/* { dg-warning "as signed due to prototype" } */
    + __builtin_ffsl (x)	/* { dg-warning "as signed due to prototype" } */
    + __builtin_parityl (x)	/* { dg-bogus "as signed due to prototype" } */
    + __builtin_popcountl (x);	/* { dg-bogus "as signed due to prototype" } */
}

int
test_sll (signed long long x)
{
  return __builtin_llabs (x)	/* { dg-bogus "as unsigned due to prototype" } */
    + __builtin_clzll (x)	/* { dg-warning "as unsigned due to prototype" } */
    + __builtin_ctzll (x)	/* { dg-warning "as unsigned due to prototype" } */
    + __builtin_clrsbll (x)	/* { dg-bogus "as unsigned due to prototype" } */
    + __builtin_ffsll (x)	/* { dg-bogus "as unsigned due to prototype" } */
    + __builtin_parityll (x)	/* { dg-warning "as unsigned due to prototype" } */
    + __builtin_popcountll (x);	/* { dg-warning "as unsigned due to prototype" } */
}

int
test_ull (unsigned long long x)
{
  return __builtin_llabs (x)	/* { dg-warning "as signed due to prototype" } */
    + __builtin_clzll (x)	/* { dg-bogus "as signed due to prototype" } */
    + __builtin_ctzll (x)	/* { dg-bogus "as signed due to prototype" } */
    + __builtin_clrsbll (x)	/* { dg-warning "as signed due to prototype" } */
    + __builtin_ffsll (x)	/* { dg-warning "as signed due to prototype" } */
    + __builtin_parityll (x)	/* { dg-bogus "as signed due to prototype" } */
    + __builtin_popcountll (x);	/* { dg-bogus "as signed due to prototype" } */
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     fn %13 @__builtin_abs(%12 <unnamed>: i32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %15 @__builtin_clz(%14 <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %17 @__builtin_ctz(%16 <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %19 @__builtin_clrsb(%18 <unnamed>: i32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %21 @__builtin_ffs(%20 <unnamed>: i32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %23 @__builtin_parity(%22 <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %25 @__builtin_popcount(%24 <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %0 @test_s(%1 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%13, read<i32>(%1)), call<i32, signature=fn(u32) -> i32>(%15, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%1)))), call<i32, signature=fn(u32) -> i32>(%17, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%1)))), call<i32, signature=fn(i32) -> i32>(%19, read<i32>(%1))), call<i32, signature=fn(i32) -> i32>(%21, read<i32>(%1))), call<i32, signature=fn(u32) -> i32>(%23, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%1)))), call<i32, signature=fn(u32) -> i32>(%25, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @test_u(%3 x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%13, reinterpret<i32, reason=arg, fits=unknown>(read<u32>(%3))), call<i32, signature=fn(u32) -> i32>(%15, read<u32>(%3))), call<i32, signature=fn(u32) -> i32>(%17, read<u32>(%3))), call<i32, signature=fn(i32) -> i32>(%19, reinterpret<i32, reason=arg, fits=unknown>(read<u32>(%3)))), call<i32, signature=fn(i32) -> i32>(%21, reinterpret<i32, reason=arg, fits=unknown>(read<u32>(%3)))), call<i32, signature=fn(u32) -> i32>(%23, read<u32>(%3))), call<i32, signature=fn(u32) -> i32>(%25, read<u32>(%3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @__builtin_labs(%26 <unnamed>: i64) -> i64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %29 @__builtin_clzl(%28 <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %31 @__builtin_ctzl(%30 <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %33 @__builtin_clrsbl(%32 <unnamed>: i64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %35 @__builtin_ffsl(%34 <unnamed>: i64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %37 @__builtin_parityl(%36 <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %39 @__builtin_popcountl(%38 <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %4 @test_sl(%5 x: i64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(call<i64, signature=fn(i64) -> i64>(%27, read<i64>(%5)), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) -> i32>(%29, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%5))))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) -> i32>(%31, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%5))))), widen<i64, reason=usual_arith>(call<i32, signature=fn(i64) -> i32>(%33, read<i64>(%5)))), widen<i64, reason=usual_arith>(call<i32, signature=fn(i64) -> i32>(%35, read<i64>(%5)))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) -> i32>(%37, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%5))))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) -> i32>(%39, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%5))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test_ul(%7 x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(call<i64, signature=fn(i64) -> i64>(%27, reinterpret<i64, reason=arg, fits=unknown>(read<u64>(%7))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) -> i32>(%29, read<u64>(%7)))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) -> i32>(%31, read<u64>(%7)))), widen<i64, reason=usual_arith>(call<i32, signature=fn(i64) -> i32>(%33, reinterpret<i64, reason=arg, fits=unknown>(read<u64>(%7))))), widen<i64, reason=usual_arith>(call<i32, signature=fn(i64) -> i32>(%35, reinterpret<i64, reason=arg, fits=unknown>(read<u64>(%7))))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) -> i32>(%37, read<u64>(%7)))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) -> i32>(%39, read<u64>(%7)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @__builtin_llabs(%40 <unnamed>: i64) -> i64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %43 @__builtin_clzll(%42 <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %45 @__builtin_ctzll(%44 <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %47 @__builtin_clrsbll(%46 <unnamed>: i64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %49 @__builtin_ffsll(%48 <unnamed>: i64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %51 @__builtin_parityll(%50 <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %53 @__builtin_popcountll(%52 <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %8 @test_sll(%9 x: i64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(call<i64, signature=fn(i64) -> i64>(%41, read<i64>(%9)), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) -> i32>(%43, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%9))))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) -> i32>(%45, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%9))))), widen<i64, reason=usual_arith>(call<i32, signature=fn(i64) -> i32>(%47, read<i64>(%9)))), widen<i64, reason=usual_arith>(call<i32, signature=fn(i64) -> i32>(%49, read<i64>(%9)))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) -> i32>(%51, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%9))))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) -> i32>(%53, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%9))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @test_ull(%11 x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(call<i64, signature=fn(i64) -> i64>(%41, reinterpret<i64, reason=arg, fits=unknown>(read<u64>(%11))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) -> i32>(%43, read<u64>(%11)))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) -> i32>(%45, read<u64>(%11)))), widen<i64, reason=usual_arith>(call<i32, signature=fn(i64) -> i32>(%47, reinterpret<i64, reason=arg, fits=unknown>(read<u64>(%11))))), widen<i64, reason=usual_arith>(call<i32, signature=fn(i64) -> i32>(%49, reinterpret<i64, reason=arg, fits=unknown>(read<u64>(%11))))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) -> i32>(%51, read<u64>(%11)))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) -> i32>(%53, read<u64>(%11)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
