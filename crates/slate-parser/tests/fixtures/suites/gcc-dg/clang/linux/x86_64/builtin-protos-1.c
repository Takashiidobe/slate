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
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abs:[0-9]+]] @__builtin_abs(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_clz:[0-9]+]] @__builtin_clz(%[[VALUE1:[0-9]+]] <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ctz:[0-9]+]] @__builtin_ctz(%[[VALUE2:[0-9]+]] <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_clrsb:[0-9]+]] @__builtin_clrsb(%[[VALUE3:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ffs:[0-9]+]] @__builtin_ffs(%[[VALUE4:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_parity:[0-9]+]] @__builtin_parity(%[[VALUE5:[0-9]+]] <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_popcount:[0-9]+]] @__builtin_popcount(%[[VALUE6:[0-9]+]] <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_test_s:[0-9]+]] @test_s(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_abs]], read<i32>(%[[VALUE_x]])), call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_clz]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x]])))), call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_ctz]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x]])))), call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_clrsb]], read<i32>(%[[VALUE_x]]))), call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_ffs]], read<i32>(%[[VALUE_x]]))), call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_parity]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x]])))), call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_popcount]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_x]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_u:[0-9]+]] @test_u(%[[VALUE_x_2:[0-9]+]] x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_abs]], reinterpret<i32, reason=arg, fits=unknown>(read<u32>(%[[VALUE_x_2]]))), call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_clz]], read<u32>(%[[VALUE_x_2]]))), call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_ctz]], read<u32>(%[[VALUE_x_2]]))), call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_clrsb]], reinterpret<i32, reason=arg, fits=unknown>(read<u32>(%[[VALUE_x_2]])))), call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_ffs]], reinterpret<i32, reason=arg, fits=unknown>(read<u32>(%[[VALUE_x_2]])))), call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_parity]], read<u32>(%[[VALUE_x_2]]))), call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_popcount]], read<u32>(%[[VALUE_x_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_labs:[0-9]+]] @__builtin_labs(%[[VALUE7:[0-9]+]] <unnamed>: i64) -> i64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_clzl:[0-9]+]] @__builtin_clzl(%[[VALUE8:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ctzl:[0-9]+]] @__builtin_ctzl(%[[VALUE9:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_clrsbl:[0-9]+]] @__builtin_clrsbl(%[[VALUE10:[0-9]+]] <unnamed>: i64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ffsl:[0-9]+]] @__builtin_ffsl(%[[VALUE11:[0-9]+]] <unnamed>: i64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_parityl:[0-9]+]] @__builtin_parityl(%[[VALUE12:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_popcountl:[0-9]+]] @__builtin_popcountl(%[[VALUE13:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_test_sl:[0-9]+]] @test_sl(%[[VALUE_x_3:[0-9]+]] x: i64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(call<i64, signature=fn(i64) ->
// DEFAULT-SAME: i64>(%[[VALUE___builtin_labs]],
// DEFAULT-SAME: read<i64>(%[[VALUE_x_3]])), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) ->
// DEFAULT-SAME: i32>(%[[VALUE___builtin_clzl]], reinterpret<u64, reason=arg,
// DEFAULT-SAME: fits=unknown>(read<i64>(%[[VALUE_x_3]]))))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) ->
// DEFAULT-SAME: i32>(%[[VALUE___builtin_ctzl]], reinterpret<u64, reason=arg,
// DEFAULT-SAME: fits=unknown>(read<i64>(%[[VALUE_x_3]]))))), widen<i64, reason=usual_arith>(call<i32, signature=fn(i64) ->
// DEFAULT-SAME: i32>(%[[VALUE___builtin_clrsbl]],
// DEFAULT-SAME: read<i64>(%[[VALUE_x_3]])))), widen<i64, reason=usual_arith>(call<i32, signature=fn(i64) ->
// DEFAULT-SAME: i32>(%[[VALUE___builtin_ffsl]],
// DEFAULT-SAME: read<i64>(%[[VALUE_x_3]])))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) ->
// DEFAULT-SAME: i32>(%[[VALUE___builtin_parityl]], reinterpret<u64, reason=arg,
// DEFAULT-SAME: fits=unknown>(read<i64>(%[[VALUE_x_3]]))))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) ->
// DEFAULT-SAME: i32>(%[[VALUE___builtin_popcountl]], reinterpret<u64, reason=arg,
// DEFAULT-SAME: fits=unknown>(read<i64>(%[[VALUE_x_3]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ul:[0-9]+]] @test_ul(%[[VALUE_x_4:[0-9]+]] x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(call<i64, signature=fn(i64) ->
// DEFAULT-SAME: i64>(%[[VALUE___builtin_labs]], reinterpret<i64, reason=arg,
// DEFAULT-SAME: fits=unknown>(read<u64>(%[[VALUE_x_4]]))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) ->
// DEFAULT-SAME: i32>(%[[VALUE___builtin_clzl]],
// DEFAULT-SAME: read<u64>(%[[VALUE_x_4]])))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) ->
// DEFAULT-SAME: i32>(%[[VALUE___builtin_ctzl]],
// DEFAULT-SAME: read<u64>(%[[VALUE_x_4]])))), widen<i64, reason=usual_arith>(call<i32, signature=fn(i64) ->
// DEFAULT-SAME: i32>(%[[VALUE___builtin_clrsbl]], reinterpret<i64, reason=arg,
// DEFAULT-SAME: fits=unknown>(read<u64>(%[[VALUE_x_4]]))))), widen<i64, reason=usual_arith>(call<i32, signature=fn(i64) ->
// DEFAULT-SAME: i32>(%[[VALUE___builtin_ffsl]], reinterpret<i64, reason=arg,
// DEFAULT-SAME: fits=unknown>(read<u64>(%[[VALUE_x_4]]))))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) ->
// DEFAULT-SAME: i32>(%[[VALUE___builtin_parityl]],
// DEFAULT-SAME: read<u64>(%[[VALUE_x_4]])))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) ->
// DEFAULT-SAME: i32>(%[[VALUE___builtin_popcountl]],
// DEFAULT-SAME: read<u64>(%[[VALUE_x_4]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_llabs:[0-9]+]] @__builtin_llabs(%[[VALUE14:[0-9]+]] <unnamed>: i64) -> i64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_clzll:[0-9]+]] @__builtin_clzll(%[[VALUE15:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ctzll:[0-9]+]] @__builtin_ctzll(%[[VALUE16:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_clrsbll:[0-9]+]] @__builtin_clrsbll(%[[VALUE17:[0-9]+]] <unnamed>: i64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ffsll:[0-9]+]] @__builtin_ffsll(%[[VALUE18:[0-9]+]] <unnamed>: i64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_parityll:[0-9]+]] @__builtin_parityll(%[[VALUE19:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_popcountll:[0-9]+]] @__builtin_popcountll(%[[VALUE20:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_test_sll:[0-9]+]] @test_sll(%[[VALUE_x_5:[0-9]+]] x: i64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(call<i64, signature=fn(i64) ->
// DEFAULT-SAME: i64>(%[[VALUE___builtin_llabs]],
// DEFAULT-SAME: read<i64>(%[[VALUE_x_5]])), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) ->
// DEFAULT-SAME: i32>(%[[VALUE___builtin_clzll]], reinterpret<u64, reason=arg,
// DEFAULT-SAME: fits=unknown>(read<i64>(%[[VALUE_x_5]]))))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) ->
// DEFAULT-SAME: i32>(%[[VALUE___builtin_ctzll]], reinterpret<u64, reason=arg,
// DEFAULT-SAME: fits=unknown>(read<i64>(%[[VALUE_x_5]]))))), widen<i64, reason=usual_arith>(call<i32, signature=fn(i64) ->
// DEFAULT-SAME: i32>(%[[VALUE___builtin_clrsbll]],
// DEFAULT-SAME: read<i64>(%[[VALUE_x_5]])))), widen<i64, reason=usual_arith>(call<i32, signature=fn(i64) ->
// DEFAULT-SAME: i32>(%[[VALUE___builtin_ffsll]],
// DEFAULT-SAME: read<i64>(%[[VALUE_x_5]])))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) ->
// DEFAULT-SAME: i32>(%[[VALUE___builtin_parityll]], reinterpret<u64, reason=arg,
// DEFAULT-SAME: fits=unknown>(read<i64>(%[[VALUE_x_5]]))))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) ->
// DEFAULT-SAME: i32>(%[[VALUE___builtin_popcountll]], reinterpret<u64, reason=arg,
// DEFAULT-SAME: fits=unknown>(read<i64>(%[[VALUE_x_5]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ull:[0-9]+]] @test_ull(%[[VALUE_x_6:[0-9]+]] x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(call<i64, signature=fn(i64) ->
// DEFAULT-SAME: i64>(%[[VALUE___builtin_llabs]], reinterpret<i64, reason=arg,
// DEFAULT-SAME: fits=unknown>(read<u64>(%[[VALUE_x_6]]))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) ->
// DEFAULT-SAME: i32>(%[[VALUE___builtin_clzll]],
// DEFAULT-SAME: read<u64>(%[[VALUE_x_6]])))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) ->
// DEFAULT-SAME: i32>(%[[VALUE___builtin_ctzll]],
// DEFAULT-SAME: read<u64>(%[[VALUE_x_6]])))), widen<i64, reason=usual_arith>(call<i32, signature=fn(i64) ->
// DEFAULT-SAME: i32>(%[[VALUE___builtin_clrsbll]], reinterpret<i64, reason=arg,
// DEFAULT-SAME: fits=unknown>(read<u64>(%[[VALUE_x_6]]))))), widen<i64, reason=usual_arith>(call<i32, signature=fn(i64) ->
// DEFAULT-SAME: i32>(%[[VALUE___builtin_ffsll]], reinterpret<i64, reason=arg,
// DEFAULT-SAME: fits=unknown>(read<u64>(%[[VALUE_x_6]]))))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) ->
// DEFAULT-SAME: i32>(%[[VALUE___builtin_parityll]],
// DEFAULT-SAME: read<u64>(%[[VALUE_x_6]])))), widen<i64, reason=usual_arith>(call<i32, signature=fn(u64) ->
// DEFAULT-SAME: i32>(%[[VALUE___builtin_popcountll]],
// DEFAULT-SAME: read<u64>(%[[VALUE_x_6]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
