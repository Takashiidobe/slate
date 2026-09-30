/* PR tree-optimization/59643 */

#define N 32

__attribute__((noinline, noclone)) void foo(double *a, double *b, double *c,
                                            double d, double e, int n) {
  int i;
  for (i = 1; i < n - 1; i++)
    a[i] = d * (b[i] + c[i] + a[i - 1] + a[i + 1]) + e * a[i];
}

double expected[] = {0.0,          10.0,         44.0,          110.0,
                     232.0,        490.0,        1020.0,        2078.0,
                     4152.0,       8314.0,       16652.0,       33326.0,
                     66664.0,      133354.0,     266748.0,      533534.0,
                     1067064.0,    2134138.0,    4268300.0,     8536622.0,
                     17073256.0,   34146538.0,   68293116.0,    136586270.0,
                     273172536.0,  546345082.0,  1092690188.0,  2185380398.0,
                     4370760808.0, 8741521642.0, 17483043324.0, 6.0};

int main() {
  int    i;
  double a[N], b[N], c[N];
  if (__DBL_MANT_DIG__ <= 35)
    return 0;
  for (i = 0; i < N; i++) {
    a[i] = (i & 3) * 2.0;
    b[i] = (i & 7) - 4;
    c[i] = i & 7;
  }
  foo(a, b, c, 2.0, 3.0, N);
  for (i = 0; i < N; i++)
    if (a[i] != expected[i])
      __builtin_abort();
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
// DEFAULT-NEXT:     global %[[VALUE_expected:[0-9]+]] expected: array<f64, 32> [storage=static] [align=16] = aggregate<array<f64, 32>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(10.0), index2 = const<f64>(44.0), index3 = const<f64>(110.0), index4 = const<f64>(232.0), index5 = const<f64>(490.0), index6 = const<f64>(1020.0), index7 = const<f64>(2078.0), index8 = const<f64>(4152.0), index9 = const<f64>(8314.0), index10 = const<f64>(16652.0), index11 = const<f64>(33326.0), index12 = const<f64>(66664.0), index13 = const<f64>(133354.0), index14 = const<f64>(266748.0), index15 = const<f64>(533534.0), index16 = const<f64>(1067064.0), index17 = const<f64>(2134138.0), index18 = const<f64>(4268300.0), index19 = const<f64>(8536622.0), index20 = const<f64>(17073256.0), index21 = const<f64>(34146538.0), index22 = const<f64>(68293116.0), index23 = const<f64>(136586270.0), index24 = const<f64>(273172536.0), index25 = const<f64>(546345082.0), index26 = const<f64>(1092690188.0), index27 = const<f64>(2185380398.0), index28 = const<f64>(4370760808.0), index29 = const<f64>(8741521642.0), index30 = const<f64>(17483043324.0), index31 = const<f64>(6.0)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_a:[0-9]+]] a: ptr<f64>, %[[VALUE_b:[0-9]+]] b: ptr<f64>, %[[VALUE_c:[0-9]+]] c: ptr<f64>, %[[VALUE_d:[0-9]+]] d: f64, %[[VALUE_e:[0-9]+]] e: f64, %[[VALUE_n:[0-9]+]] n: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(1));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), sub<i32, overflow=ub>(read<i32>(%[[VALUE_n]]), const<i32>(1)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_a]]), read<i32>(%[[VALUE_i]]))), add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_d]]), add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_b]]), read<i32>(%[[VALUE_i]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_c]]), read<i32>(%[[VALUE_i]]))))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_a]]), sub<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(1)))))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_a]]), add<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(1))))))), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_e]]), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_a]]), read<i32>(%[[VALUE_i]])))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: array<f64, 32> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_b_2:[0-9]+]] b: array<f64, 32> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_c_2:[0-9]+]] c: array<f64, 32> [storage=automatic] [align=16];
// DEFAULT-NEXT:         if le<i32>(const<i32>(53), const<i32>(35))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(32)>(%[[VALUE_a_2]]), read<i32>(%[[VALUE_i_2]]))), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(and<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(3))), const<f64>(2.0)));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(32)>(%[[VALUE_b_2]]), read<i32>(%[[VALUE_i_2]]))), int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(sub<i32, overflow=ub>(and<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(7)), const<i32>(4))));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(32)>(%[[VALUE_c_2]]), read<i32>(%[[VALUE_i_2]]))), int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(and<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(7))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<f64>, ptr<f64>, ptr<f64>, f64, f64, i32) -> void>(%[[VALUE_foo]], array_decay<ptr<f64>, length=Some(32)>(%[[VALUE_a_2]]), array_decay<ptr<f64>, length=Some(32)>(%[[VALUE_b_2]]), array_decay<ptr<f64>, length=Some(32)>(%[[VALUE_c_2]]), const<f64>(2.0), const<f64>(3.0), const<i32>(32));
// DEFAULT-NEXT:         for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<f64, exceptions=observable>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(32)>(%[[VALUE_a_2]]), read<i32>(%[[VALUE_i_2]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(32)>(%[[VALUE_expected]]), read<i32>(%[[VALUE_i_2]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
