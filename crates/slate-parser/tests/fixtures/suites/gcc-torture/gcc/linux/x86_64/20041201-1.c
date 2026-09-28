/* PR rtl-opt/15289 */

typedef struct {
  _Complex char a;
  _Complex char b;
} Scc2;

Scc2 s = {1 + 2i, 3 + 4i};

int checkScc2(Scc2 s) { return s.a != 1 + 2i || s.b != 3 + 4i; }

int main(void) { return checkScc2(s); }



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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 a: complex<i8>;
// DEFAULT-NEXT:         field1 b: complex<i8>;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type1 Scc2 = @type0;
// DEFAULT-NEXT:     global %2 s: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = complex_convert<complex<i8>, reason=assign, fits=unknown>(add<complex<i32>, complex=true, overflow=ub>(const<i32>(1), aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(2)))), field1 = complex_convert<complex<i8>, reason=assign, fits=unknown>(add<complex<i32>, complex=true, overflow=ub>(const<i32>(3), aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(4))))) [linkage=external];
// DEFAULT-NEXT:     fn %3 @checkScc2(%4 s: @type0) -> i32 [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_or<bool>(ne<complex<i32>>(complex_convert<complex<i32>, reason=usual_arith>(read<complex<i8>>(field0(%4))), add<complex<i32>, complex=true, overflow=ub>(const<i32>(1), aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(2)))), ne<complex<i32>>(complex_convert<complex<i32>, reason=usual_arith>(read<complex<i8>>(field1(%4))), add<complex<i32>, complex=true, overflow=ub>(const<i32>(3), aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(4))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         return call<i32, signature=fn(@type0) -> i32, abi=sysv64(native_c) -> scalar>(%3, copy<@type0, reason=arg>(read<@type0>(%2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
