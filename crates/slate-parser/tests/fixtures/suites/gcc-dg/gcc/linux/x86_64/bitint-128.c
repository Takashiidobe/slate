/* PR tree-optimization/124826 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-O2" } */

typedef float V __attribute__((vector_size (16 * sizeof (float))));

union {
#if __BITINT_MAXWIDTH__ >= 256
  _BitInt(256) b;
#endif
  V v;
} u;

void
foo (int c)
{
  u.v -= c ? 0.f : 1.f;
#if __BITINT_MAXWIDTH__ >= 256
  u.b *= c;
#endif
}

int
main ()
{
  foo (2);
}

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type0 V = vector<f32, 16>;
// DEFAULT-NEXT:     type @type1 = union {
// DEFAULT-NEXT:         field0 b: i256b;
// DEFAULT-NEXT:         field1 v: vector<f32, 16>;
// DEFAULT-NEXT:     } [size=64, align=64, offsets=[0, 0]];
// DEFAULT-NEXT:     global %2 u: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo(%4 c: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6: vector<f32, 16> [synthetic] = read<vector<f32, 16>>(field1(%2));
// DEFAULT-NEXT:         let %7: vector<f32, 16> [synthetic] = sub<vector<f32, 16>, elementwise=true, rounding=nearest_even, exceptions=observable, contract=fast>(read<vector<f32, 16>>(%6), vector_splat<vector<f32, 16>, reason=usual_arith>(conditional<f32>(ne<i32>(read<i32>(%4), const<i32>(0)), const<f32>(0.0), const<f32>(1.0))));
// DEFAULT-NEXT:         write<vector<f32, 16>>(field1(%2), read<vector<f32, 16>>(%7));
// DEFAULT-NEXT:         let %8: i256b [synthetic] = read<i256b>(field0(%2));
// DEFAULT-NEXT:         let %9: i256b [synthetic] = mul<i256b, overflow=ub>(read<i256b>(%8), widen<i256b, reason=usual_arith>(read<i32>(%4)));
// DEFAULT-NEXT:         write<i256b>(field0(%2), read<i256b>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%3, const<i32>(2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
