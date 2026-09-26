/* { dg-do run } */

double s[4] = {1.0, 2.0, 3.0, 4.0}, pol_x[2] = {5.0, 6.0};

__attribute__((noinline)) int foo(void) {
  double coef_x[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  int    lxp       = 0;
  if (lxp <= 1)
    do {
      double t = pol_x[lxp];
      long   S;
      long   l = lxp * 4L - 1;
      for (S = 1; S <= 4; S++)
        coef_x[S + l] = coef_x[S + l] + s[S - 1] * t;
    } while (lxp++ != 1);
  asm volatile("" : : "r"(coef_x) : "memory");
  for (lxp = 0; lxp < 8; lxp++)
    if (coef_x[lxp] != ((lxp & 3) + 1) * (5.0 + (lxp >= 4)))
      __builtin_abort();
  return 1;
}

int
main() {
  asm volatile("" : : : "memory");
  if (!foo())
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
// DEFAULT-NEXT:     global %0 s: array<f64, 4> [storage=static] [align=16] = aggregate<array<f64, 4>, zero_fill=false>(index0 = const<f64>(1.0), index1 = const<f64>(2.0), index2 = const<f64>(3.0), index3 = const<f64>(4.0)) [linkage=external];
// DEFAULT-NEXT:     global %1 pol_x: array<f64, 2> [storage=static] [align=16] = aggregate<array<f64, 2>, zero_fill=false>(index0 = const<f64>(5.0), index1 = const<f64>(6.0)) [linkage=external];
// DEFAULT-NEXT:     fn %12 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @foo() -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 coef_x: array<f64, 8> [storage=automatic] [align=16] = aggregate<array<f64, 8>, zero_fill=false>(index0 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), index1 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), index2 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), index3 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), index4 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), index5 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), index6 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), index7 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)));
// DEFAULT-NEXT:         let %4 lxp: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         if le<i32>(read<i32>(%4), const<i32>(1))
// DEFAULT-NEXT:             do %9
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %5 t: f64 [storage=automatic] = read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(2)>(%1), read<i32>(%4))));
// DEFAULT-NEXT:                     let %6 S: i64 [storage=automatic];
// DEFAULT-NEXT:                     let %7 l: i64 [storage=automatic] = sub<i64, overflow=ub>(mul<i64, overflow=ub>(widen<i64, reason=usual_arith>(read<i32>(%4)), const<i64>(4)), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                     for %10
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i64>(%6, widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:                         condition: le<i64>(read<i64>(%6), widen<i64, reason=usual_arith>(const<i32>(4)))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %13: i64 [synthetic] = read<i64>(%6);
// DEFAULT-NEXT:                             let %14: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%13), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                             write<i64>(%6, read<i64>(%14));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%3), add<i64, overflow=ub>(read<i64>(%6), read<i64>(%7)))), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%3), add<i64, overflow=ub>(read<i64>(%6), read<i64>(%7))))), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(4)>(%0), sub<i64, overflow=ub>(read<i64>(%6), widen<i64, reason=usual_arith>(const<i32>(1)))))), read<f64>(%5))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             while {
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%16));
// DEFAULT-NEXT:                 yield ne<i32>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:             };
// DEFAULT-NEXT:         asm volatile "" {
// DEFAULT-NEXT:             in 0 "r" array_decay<ptr<f64>, length=Some(8)>(%3);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         for %11
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%4, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%4), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%18));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(8)>(%3), read<i32>(%4)))), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(add<i32, overflow=ub>(and<i32>(read<i32>(%4), const<i32>(3)), const<i32>(1))), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(5.0), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(from_bool<i32, reason=promotion>(ge<i32>(read<i32>(%4), const<i32>(4)))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         asm volatile "" {
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%2), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
