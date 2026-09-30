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
// DEFAULT-NEXT:     type @type[[TYPE_V:[0-9]+]] V = vector<f32, 16>;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 b: i256b;
// DEFAULT-NEXT:         field1 v: vector<f32, 16>;
// DEFAULT-NEXT:     } [size=64, align=64, offsets=[0, 0]];
// DEFAULT-NEXT:     global %[[VALUE_u:[0-9]+]] u: @type[[TYPE0]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_c:[0-9]+]] c: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: vector<f32, 16> [synthetic] = read<vector<f32, 16>>(field1(%[[VALUE_u]]));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: vector<f32, 16> [synthetic] = sub<vector<f32, 16>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 16>>(%[[VALUE0]]), vector_splat<vector<f32, 16>, reason=usual_arith>(conditional<f32>(ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(0)), const<f32>(0.0), const<f32>(1.0))));
// DEFAULT-NEXT:         write<vector<f32, 16>>(field1(%[[VALUE_u]]), read<vector<f32, 16>>(%[[VALUE1]]));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i256b [synthetic] = read<i256b>(field0(%[[VALUE_u]]));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i256b [synthetic] = mul<i256b, overflow=ub>(read<i256b>(%[[VALUE2]]), widen<i256b, reason=usual_arith>(read<i32>(%[[VALUE_c]])));
// DEFAULT-NEXT:         write<i256b>(field0(%[[VALUE_u]]), read<i256b>(%[[VALUE3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_foo]], const<i32>(2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
