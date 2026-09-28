// SLATE-FILECHECK-DEFINES DEFAULT

double bar(void), c;
int foo(void) {
	double a, b;
	int i = bar() + bar();
	a = i; i += 1; a += 0.1; i = c + i;
	return i;
}

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
// DEFAULT-NEXT:     global %1 c: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @bar() -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 a: f64 [storage=automatic];
// DEFAULT-NEXT:         let %4 b: f64 [storage=automatic];
// DEFAULT-NEXT:         let %5 i: i32 [storage=automatic] = float_to_int<i32, reason=assign, out_of_range=ub, exceptions=observable>(add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn() -> f64>(%0), call<f64, signature=fn() -> f64>(%0)));
// DEFAULT-NEXT:         write<f64>(%3, int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(read<i32>(%5)));
// DEFAULT-NEXT:         let %6: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:         let %7: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%6), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%5, read<i32>(%7));
// DEFAULT-NEXT:         let %8: f64 [synthetic] = read<f64>(%3);
// DEFAULT-NEXT:         let %9: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%8), const<f64>(0.1));
// DEFAULT-NEXT:         write<f64>(%3, read<f64>(%9));
// DEFAULT-NEXT:         write<i32>(%5, float_to_int<i32, reason=assign, out_of_range=ub, exceptions=observable>(add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%1), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(read<i32>(%5)))));
// DEFAULT-NEXT:         return read<i32>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
